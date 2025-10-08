#include "scheduler.h"

.globl cpu_switch_to
cpu_switch_to:
    add x8, x0, #0 // the offset of the cpu context inside task_struct
    mov x9, sp // caller-saved, we can use them!

    // Move the registers into cpu_ctx in the task_struct
    stp x19, x20, [x8], #16 // store callee-saved registers
    stp x21, x22, [x8], #16
    stp x23, x24, [x8], #16
    stp x25, x26, [x8], #16
    stp x27, x28, [x8], #16
    stp x29, x9, [x8], #16 // NOTE: sp moved before lr cause that's the order in cpu_ctx
    str x30, [x8]

    // Load the register from the cpu_ctx of the 'next' task_struct
    add x8, x1, #0
    ldp x19, x20, [x8], #16 // restore callee-saved registers
    ldp x21, x22, [x8], #16
    ldp x23, x24, [x8], #16
    ldp x25, x26, [x8], #16
    ldp x27, x28, [x8], #16
    ldp x29, x9, [x8], #16
    ldr x30, [x8]
    mov sp, x9
    ret
