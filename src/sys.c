#include "fork.h"
#include "mm.h"
#include "printf.h"
#include "scheduler.h"
#include "utils.h"
#include "fs.h"

void sys_write(char* buf) { printf("%s", buf); }

int sys_clone(unsigned long stack) { return copy_process(0, 0, 0, stack); }

unsigned long sys_malloc() {
    unsigned long addr = get_free_page();
    if (!addr) {
        return -1;
    }
    return addr;
}

void sys_exit() {
    // exit_process();
}

int sys_open(const char* path, int flags) {
    int fd = -1;
    for (int i = 0; i < MAX_FILES_PER_PROCESS; i++) {
        if (current->file_table[i] == 0) {
            fd = i;
            break;
        }
    }
    if (fd == -1) {
        return -1; // No free file descriptors
    }

    struct file* f = NULL;
    if (vfs_open_at(current->cwd, path, flags, &f) != 0) {
        return -1; // Failed to open file
    }

    current->file_table[fd] = f;
    return fd;
}

int sys_write_file(int fd, const void* buf, int len) {
    if (fd < 0 || fd >= MAX_FILES_PER_PROCESS || current->file_table[fd] == 0) {
        return -1;
    }
    return vfs_write(current->file_table[fd], buf, len);
}

int sys_read_file(int fd, void* buf, int len) {
    if (fd < 0 || fd >= MAX_FILES_PER_PROCESS || current->file_table[fd] == 0) {
        return -1;
    }
    return vfs_read(current->file_table[fd], buf, len);
}

int sys_close(int fd) {
    if (fd < 0 || fd >= MAX_FILES_PER_PROCESS || current->file_table[fd] == 0) {
        return -1;
    }
    vfs_close(current->file_table[fd]);
    current->file_table[fd] = 0;
    return 0;
}

void* const sys_call_table[] = {
    sys_write, 
    sys_malloc, 
    sys_clone, 
    sys_exit,
    sys_open,
    sys_write_file,
    sys_read_file,
    sys_close
};