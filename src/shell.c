#include "shell.h"
#include "printf.h"
#include <stdint.h>

#include "uart.h"

#include "string.h"
#include "fs.h"
#include "tmpfs.h"

static struct vnode* cwd;

struct redirect_buffer {
    char *buf;
    int cursor;
    int max_len;
};

static void redirect_putc(void *p, char c) {
    struct redirect_buffer *rb = (struct redirect_buffer *)p;
    if (rb->cursor < rb->max_len - 1) {
        rb->buf[rb->cursor++] = c;
    }
}

static void cmd_help() {
    printf("TycheOS Shell Commands\r\n");
    printf("  help        - Display this help message\r\n");
    printf("  clear       - Clear the terminal screen\r\n");
    printf("  echo [msg]  - Print [msg] back to the terminal (supports > redirection)\r\n");
    printf("  ls [path]   - List directory contents (defaults to current)\r\n");
    printf("  pwd         - Print name of current/working directory\r\n");
    printf("  mkdir [name]- Create a directory\r\n");
    printf("  touch [name]- Create a file\r\n");
    printf("  cat [name]  - Print file content\r\n");
    printf("  cd [path]   - Change directory (supports . and ..)\r\n");
}

static void cmd_echo(const char *input) {
    if (input[4] == '\0') {
        printf("\r\n");
    } else {
        printf("%s\r\n", input + 5);
    }
}

static void cmd_clear() { printf("\x1b[2J\x1b[H"); }

static void cmd_ls(const char* path) {
    // trim spaces from path
    while (*path == ' ') {
        path++;
    }
    
    struct vnode* target = cwd;
    if (strlen(path) > 0) {
        if (vfs_lookup_at(cwd, path, &target) != 0) {
            printf("ls: %s: No such file or directory\r\n", path);
            return;
        }
    }

    struct tmpfs_node* node = (struct tmpfs_node*)target->internal;
    if (node->type != TMPFS_DIR) {
        printf("ls: not a directory: %s\r\n", target->name);
        return;
    }

    for (int i = 0; i < node->num_children; i++) {
        struct tmpfs_node* child = node->children[i];
        if (child->type == TMPFS_DIR) {
            printf("[dir]  %s\r\n", child->vnode.name);
        } else {
            printf("[file] %s  (%d bytes)\r\n", child->vnode.name, child->content_size);
        }
    }
}

static void get_cwd_path(struct vnode* node, char* path) {
    struct vnode* trace[64];
    int count = 0;
    struct vnode* curr = node;
    
    // walk up to root [safe bounds check]
    while (curr->parent != curr && count < 64) {
        trace[count++] = curr;
        curr = curr->parent;
    }
    
    strcpy(path, "/");
    int len = 1;
    
    for (int i = count - 1; i >= 0; i--) {
        int name_len = strlen(trace[i]->name);
        if (len + name_len + 2 >= MAX_PATH_LEN) {
            break; // prevent buffer overflow
        }
        if (len > 1) {
            strcat(path, "/");
            len++;
        }
        strcat(path, trace[i]->name);
        len += name_len;
    }
}

static void cmd_pwd() {
    char path[MAX_PATH_LEN];
    get_cwd_path(cwd, path);
    printf("%s\r\n", path);
}

static void cmd_mkdir(const char* path) {
    if (path[0] == '\0') {
        printf("mkdir: missing operand\r\n");
        return;
    }
    if (vfs_create_at(cwd, path, VFS_DIR) != 0) {
        printf("mkdir: cannot create directory: %s\r\n", path);
    }
}

static void cmd_touch(const char* path) {
    if (path[0] == '\0') {
        printf("touch: missing operand\r\n");
        return;
    }
    if (vfs_create_at(cwd, path, VFS_FILE) != 0) {
        printf("touch: cannot create file: %s\r\n", path);
    }
}

static void cmd_cat(const char* path) {
    if (path[0] == '\0') {
        printf("cat: missing operand\r\n");
        return;
    }
    struct file* f;
    if (vfs_open_at(cwd, path, 0, &f) != 0) {
        printf("cat: cannot open file: %s\r\n", path);
        return;
    }

    struct tmpfs_node* tmpfs_node = (struct tmpfs_node*)f->vnode->internal;
    if (tmpfs_node->type == TMPFS_DIR) {
        printf("cat: %s: Is a directory\r\n", path);
        vfs_close(f);
        return;
    }

    char buf[1025];
    int n = vfs_read(f, buf, 1024);
    if (n < 0) {
        printf("cat: read error\r\n");
    } else {
        buf[n] = '\0';
        printf("%s\r\n", buf);
    }
    vfs_close(f);
}

static void cmd_cd(const char* path) {
    if (path[0] == '\0') {
        // cd to root if no argument
        cwd = vfs_get_root();
        return;
    }
    struct vnode* node;
    if (vfs_lookup_at(cwd, path, &node) != 0) {
        printf("cd: no such file or directory: %s\r\n", path);
        return;
    }
    struct tmpfs_node* tmpfs_node = (struct tmpfs_node*)node->internal;
    if (tmpfs_node->type != TMPFS_DIR) {
        printf("cd: not a directory: %s\r\n", path);
        return;
    }
    cwd = node;
}


static void run_cmd(char *cmd) {
    // trim trailing spaces of cmd
    int cmd_len = strlen(cmd);
    while (cmd_len > 0 && cmd[cmd_len - 1] == ' ') {
        cmd[cmd_len - 1] = '\0';
        cmd_len--;
    }

    // Check for redirection
    char *redir = strchr(cmd, '>');
    char *redir_file = NULL;
    
    if (redir) {
        *redir = '\0';
        redir_file = redir + 1;
        // trim leading for filename
        while (*redir_file == ' ') {
            redir_file++;
        }
        // trim trailing for filename
        int fn_len = strlen(redir_file);
        while (fn_len > 0 && redir_file[fn_len - 1] == ' ') {
            redir_file[fn_len - 1] = '\0';
            fn_len--;
        }
        // trim trailing spaces of cmd (whatevers before '>')
        char *end = redir - 1;
        while (end >= cmd && *end == ' ') {
            *end = '\0';
            end--;
        }
    }

    if (redir && redir_file[0] == '\0') {
        printf("shell: syntax error near unexpected token 'newline'\r\n");
        return;
    }

    char redir_buf[4096];
    struct redirect_buffer rb = {redir_buf, 0, sizeof(redir_buf)};
    
    if (redir_file) {
        init_printf(&rb, redirect_putc);
    }

    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strncmp(cmd, "echo", 4) == 0 && (cmd[4] == ' ' || cmd[4] == '\0')) {
        cmd_echo(cmd);
    } else if (strcmp(cmd, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(cmd, "ls") == 0 || strncmp(cmd, "ls ", 3) == 0) {
        cmd_ls(cmd + (cmd[2] == ' ' ? 3 : 2));
    } else if (strcmp(cmd, "pwd") == 0) {
        cmd_pwd();
    } else if (strncmp(cmd, "mkdir ", 6) == 0) {
        cmd_mkdir(cmd + 6);
    } else if (strncmp(cmd, "touch ", 6) == 0) {
        cmd_touch(cmd + 6);
    } else if (strncmp(cmd, "cat ", 4) == 0) {
        cmd_cat(cmd + 4);
    } else if (strncmp(cmd, "cd ", 3) == 0 || strcmp(cmd, "cd") == 0) {
        cmd_cd(cmd + (cmd[2] == ' ' ? 3 : 2));
    } else if (strcmp(cmd, "") == 0) {
        // do nothing
    } else {
        printf("Unknown Command: %s\r\n", cmd);
    }

    if (redir_file) {
        init_printf(0, putc);
        redir_buf[rb.cursor] = '\0';
        
        struct file *f = NULL;
        struct vnode *node = NULL;
        if (vfs_lookup_at(cwd, redir_file, &node) != 0) {
            if (vfs_create_at(cwd, redir_file, VFS_FILE) != 0) {
                printf("shell: cannot create %s\r\n", redir_file);
                return;
            }
        }
        
        if (vfs_open_at(cwd, redir_file, 0, &f) != 0) {
            printf("shell: cannot open %s\r\n", redir_file);
            return;
        }
        
        struct tmpfs_node *tmp_node = (struct tmpfs_node *)f->vnode->internal;
        if (tmp_node->type == TMPFS_DIR) {
            printf("shell: %s: Is a directory\r\n", redir_file);
            vfs_close(f);
            return;
        }
        
        int written = vfs_write(f, rb.buf, rb.cursor);
        if (written < 0) {
            printf("shell: error writing to %s\r\n", redir_file);
        }
        vfs_close(f);
    }
}

void shell() {
    cwd = vfs_get_root();
    printf("\r\nTycheOS Shell\r\n");

    char cmd_buffer[256];
    int index = 0;

    while (1) {
        char path[MAX_PATH_LEN];
        get_cwd_path(cwd, path);
        printf("%s> ", path);

        index = 0;
        while (1) {
            char c = uart_getc();

            // backspace
            if (c == '\b' || c == 127) {
                if (index > 0) {
                    index--;
                    uart_puts("\b \b");
                }
            }
            // enter
            else if (c == '\r') {
                cmd_buffer[index] = '\0';
                printf("\r\n");

                if (index > 0) {
                    run_cmd(cmd_buffer);
                }
                break; // break from inner loop to reprint prompt
            } else {
                // make sure we don't overflow the buffer
                if (index < (int)sizeof(cmd_buffer) - 1) {
                    cmd_buffer[index++] = c;
                    uart_putc(c);
                }
            }
        }
    }
}
