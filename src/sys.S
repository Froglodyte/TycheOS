#include "sys.h"

.globl call_sys_write
call_sys_write:
    mov w8, #SYS_WRITE_NUMBER
    svc #0
    ret

.globl call_sys_malloc
call_sys_malloc:
    mov w8, #SYS_MALLOC_NUMBER
    svc #0
    ret

.globl call_sys_clone
call_sys_clone:
    mov w8, #SYS_CLONE_NUMBER
    svc #0
    ret

.globl call_sys_exit
call_sys_exit:
    mov w8, #SYS_EXIT_NUMBER
    svc #0
    ret

.globl call_sys_open
call_sys_open:
    mov w8, #SYS_OPEN_NUMBER
    svc #0
    ret

.globl call_sys_write_file
call_sys_write_file:
    mov w8, #SYS_WRITE_FILE_NUMBER
    svc #0
    ret

.globl call_sys_read_file
call_sys_read_file:
    mov w8, #SYS_READ_FILE_NUMBER
    svc #0
    ret

.globl call_sys_close
call_sys_close:
    mov w8, #SYS_CLOSE_NUMBER
    svc #0
    ret
