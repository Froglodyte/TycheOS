#ifndef SYS_H
#define SYS_H

#define NR_SYSCALLS 8

#define SYS_WRITE_NUMBER 0
#define SYS_MALLOC_NUMBER 1
#define SYS_CLONE_NUMBER 2
#define SYS_EXIT_NUMBER 3
#define SYS_OPEN_NUMBER 4
#define SYS_WRITE_FILE_NUMBER 5
#define SYS_READ_FILE_NUMBER 6
#define SYS_CLOSE_NUMBER 7

#ifndef __ASSEMBLER__

void sys_write(char* buf);
int sys_fork();
int sys_open(const char* path, int flags);
int sys_write_file(int fd, const void* buf, int len);
int sys_read_file(int fd, void* buf, int len);
int sys_close(int fd);

void call_sys_write(char* buf);
int call_sys_clone(unsigned long fn, unsigned long arg, unsigned long stack);
unsigned long call_sys_malloc();
void call_sys_exit();
int call_sys_open(const char* path, int flags);
int call_sys_write_file(int fd, const void* buf, int len);
int call_sys_read_file(int fd, void* buf, int len);
int call_sys_close(int fd);

#endif
#endif