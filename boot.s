.section ".text.boot"

.globl _start
_start:
    mrs    x0, mpidr_el1        
    and    x0, x0,#0xFF
    cbz    x0, master   // primary core jumps to master
    b    hang   // hangs the other 3 cores

master:
    // stack pointer
    ldr x0, = _start
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_end
    sub x1, x1, x0  // calculates bss size

clear_bss:
    cbz x1, jump_to_c
    str xzr, [x0], #8
    sub x1, x1, #8
    b   check_bss

check_bss:
    cmp x0, x1
    b.lo clear_bss_loop // if the current address is lower, loop again
    bl main

clear_bss_loop:
    str xzr, [x0], #8

jump_to_c:
    bl main

// infinite recursion to hang the other 3 cores.
hang: 
    wfe
    b hang

halt:
    wfi
    b halt