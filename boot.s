.section ".bss"
.align 16
stack_bottom:
.skip 16384  // allocate 16KB for the stack
stack_top:

.section ".text.boot"

.globl _start
_start:
    mrs    x0, mpidr_el1        
    and    x0, x0,#0xFF
    cbz    x0, master   // primary core jumps to master
    b    hang   // hangs the other 3 cores

master:
    // stack pointer
    ldr x0, =stack_top
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_end
    sub x1, x1, x0  // calculates bss size

clear_bss_loop:
    cbz x1, jump_to_c
    str xzr, [x0], #8
    sub x1, x1, #8
    b   clear_bss_loop

jump_to_c:
    bl main

// infinite recursion to hang the other 3 cores.
hang:
    wfe
    b hang

// halts main core when everything is done
halt:
    wfi
    b halt