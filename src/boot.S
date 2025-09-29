#include "arm/sysregs.h"

.section ".bss"
.align 16
stack_bottom:
    .skip 16384 // allocate 16KB for the kernel stack
stack_top:

.section ".text.boot"

.globl _start
_start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbz x0, master // primary core jumps to master
    b hang // hangs the other 3 cores

master:
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0

    ldr x0, =HCR_VALUE
    msr hcr_el2, x0

    ldr x0, =SCR_VALUE
    msr scr_el3, x0

    ldr x0, =SPSR_VALUE
    msr spsr_el3, x0

    adr x0, el1_entry
    msr elr_el3, x0

    eret

el1_entry:
    adr x0, __bss_start
    adr x1, __bss_end
    sub x1, x1, x0
    bl memzero

    ldr x0, =stack_top
    mov sp, x0

    bl main
    b hang // should never reach here

// infinite recursion to hang the other 3 cores.
hang:
    b hang
