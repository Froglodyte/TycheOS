#include "shell.h"
#include "printf.h"
#include <stdint.h>

#include "uart.h"

#include "string.h"
#include "fs.h"
#include "tmpfs.h"

static struct vnode* cwd;

static void cmd_help() {
    printf("TycheOS Shell Commands\r\n");
    printf("  help        - Display this help message\r\n");
    printf("  clear       - Clear the terminal screen\r\n");
    printf("  echo [msg]  - Print [msg] back to the terminal\r\n");
    printf("  ls          - List directory contents\r\n");
    printf("  pwd         - Print name of current/working directory\r\n");
    printf("  mkdir [name]- Create a directory\r\n");
    printf("  touch [name]- Create a file\r\n");
    printf("  cat [name]  - Print file content\r\n");
    printf("  cd [path]   - Change directory\r\n");
}

static void cmd_echo(const char *input) {
    printf("%s\r\n", input + 5);
}

static void cmd_clear() { printf("\x1b[2J\x1b[H"); }

static void cmd_ls() {
    struct tmpfs_node* node = (struct tmpfs_node*)cwd->internal;
    if (node->type != TMPFS_DIR) {
        printf("ls: not a directory: %s\r\n", cwd->name);
        return;
    }

    for (int i = 0; i < node->num_children; i++) {
        struct tmpfs_node* child = node->children[i];
        if (child->type == TMPFS_DIR) {
            printf("[directory] %s\r\n", child->vnode.name);
        } else {
            printf("[file] %s\r\n", child->vnode.name);
        }
    }
}

static void get_cwd_path(struct vnode* node, char* path) {
    if (node->parent == node) {
        strcpy(path, "/");
        return;
    }

    get_cwd_path(node->parent, path);
    if (strlen(path) > 1) {
        strcat(path, "/");
    }
    strcat(path, node->name);
}

static void cmd_pwd() {
    char path[MAX_PATH_LEN];
    get_cwd_path(cwd, path);
    printf("%s\r\n", path);
}

static void cmd_mkdir(const char* path) {
    if (vfs_create(path, VFS_DIR) != 0) {
        printf("mkdir: cannot create directory: %s\r\n", path);
    }
}

static void cmd_touch(const char* path) {
    if (vfs_create(path, VFS_FILE) != 0) {
        printf("touch: cannot create file: %s\r\n", path);
    }
}

static void cmd_cat(const char* path) {
    struct file* f;
    if (vfs_open(path, 0, &f) != 0) {
        printf("cat: cannot open file: %s\r\n", path);
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
    struct vnode* node;
    if (vfs_lookup(path, &node) != 0) {
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


static void run_cmd(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        cmd_echo(cmd);
    } else if (strcmp(cmd, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(cmd, "ls") == 0) {
        cmd_ls();
    } else if (strcmp(cmd, "pwd") == 0) {
        cmd_pwd();
    } else if (strncmp(cmd, "mkdir ", 6) == 0) {
        cmd_mkdir(cmd + 6);
    } else if (strncmp(cmd, "touch ", 6) == 0) {
        cmd_touch(cmd + 6);
    } else if (strncmp(cmd, "cat ", 4) == 0) {
        cmd_cat(cmd + 4);
    } else if (strncmp(cmd, "cd ", 3) == 0) {
        cmd_cd(cmd + 3);
    }
    else {
        printf("Unknown Command: %s\r\n", cmd);
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
                if (index < sizeof(cmd_buffer) - 1) {
                    cmd_buffer[index++] = c;
                    uart_putc(c);
                }
            }
        }
    }
}
