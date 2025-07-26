.section ".text.boot"

.globl _entry
_entry:
    mrs    x0, mpidr_el1        
    and    x0, x0,#0xFF
    cbz    x0, master // primary core jumps to master
    b    hang // hangs the other 3 cores

master:
    adr    x0, bss_begin
    adr    x1, bss_end
    sub    x1, x1, x0 // get bss size

clear_bss:
    cbz x1, jump_to_c
    str xzr, [x0], #8
    sub x1, x1, #8
    b   clear_bss

jump_to_c:
    // move to main kernel from here
    // also initialise stack pointer

// infinite recursion to hang the other 3 cores.
hang: 
    b hang