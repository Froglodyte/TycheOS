# ARM Assembly Learning

## Data alignment and word size

-   Word size generally signifies maximum no of bytes a register can hold at once. The memory is aligned in such a fashion that, to store a value, it must be aligned according to the word size boundaries, else it may cause perf issues or may even cause a CPU exception
-   See structure padding and packing in C!
-   Hence data must be aligned in the memory according to the word size. `.align`, `.p2align`, `.balign` directives help us achieve exactly this
-   It is 64 bits for this project!

## Stack Pointer and the Stack

-   The **stack** is a **region of memory** used for temporary storage during program execution.
-   Common uses:

    -   Storing **local variables**
    -   **Saving register states** (for function calls or interrupts)
    -   **Passing arguments** (beyond the first 8 in registers)
    -   **Returning addresses**

-   In AArch64 the stack is `full descending` stack. Hence the stack-pointer refers to piece of memory where the last element was stored.
-   To push and pop:
    ```arm
    STP X29, X30, [SP, #-16]!   ; PUSH X29, X30 (store pair, pre-decrement SP)
    LDP X29, X30, [SP], #16     ; POP X29, X30 (load pair, post-increment SP)(store pair, pre-decrement SP)
    ```

## General Purpose Registers for x86 - 32 bits (https://x86re.com/1.html)

-   Data Registers:
    -   EAX
    -   EBX
    -   ECX
    -   EDX
-   Index Registers:
    -   ESI
    -   EDI
-   Pointers:
    -   ESP
    -   EBP
-   Flags

## General Purpose Registers - AArch64

-   There are 31 64 bit X-registers (x0 - x31) or 31 32-bit registers (w0 - w31), both of which are separate ways of looking at the same register. w is the bottom 32 bits of the x register
-   There is a separate set of 32 registers used for floating point and vector operations. These registers are 128 bit, but like general purpose registers, can be accessed in several ways.
    -   Bx -> 8 bits (Byte)
    -   Hx -> 16 bits (Halfword)
    -   Sx -> 32 bits (Singleword)
    -   Dx -> 64 bits (Doubleword)
    -   Qx -> 128 bits (Quadword)
-   These regiters are also referred to as Vx with appropriate suffixes
    -   `FADD V0.2D, V1.2D, V2.2D` performs floating point operations, by treating 2 doublewords as separate
    -   `ADD V0.4D, V1.4S, V2.4S` performs integer addition by treating 4 singlewords as independent
-   ARMv8-A calls these registers (Qx, Vx.\<suffix\>) as NEON registers.

## More on registers

-   PC (not a register!)
-   FP (x29)
-   SP (kinda like xzr)
-   SPSel: Allows the Stack Pointer to be selected between SP_EL0 and SP_ELx.
-   CurrentEL: Current exception level
-   PS
    -   CPSR: Current Program Status Register (NZCV flags. All the logical operations results are stored in them)
    -   SPSR_ELn: Saved Program Status Register
-   SCTLR_ELn: System Control Register
-   ELR_Eln: Exception Link Register (x30)  
    (El_n is the level at which the exception level is handled at)
-   xzr: a register that holds the value zero (writes to this register are ignored)

## AAPCS (Procedure call standard for the ARM 64-bit architecture)

-   Defines the ABI, Application Binary Interface that generated binaries need to follow
-   Ann application binary interface (ABI) is an intercae exposed by software that is defined for in-process machine code access.
-   In other words it defines function calling conventions
    -   How parameters should be passed?
    -   How the return value should be returned?
    -   How the stack should be managed between the caller and the callee
-   For the purpose of this project we only need to know a few important rules
    -   Registers x0-x7 must be used for passing parameters
    -   Register x0 is used for returning the return value
    -   x9 to x15 are caller saved registers
    -   x19 - x28 are callee saved registers

## Some important points about armasm

-   Load: `ldr x0, [source]`
-   Move: `move x0, x1`
-   Store: `str
-   Move to System Register: `msr sctlr_el1, x0`
-   Move from register to system register `mrs sctlr_el1, x0`
-   AArch64 is LP model, long and pointer are 64 bits!
-   Instructions are still 4 bytes
-   Make sure to know about the literal pool and `adr` concepts
    -   So putting stuff as a direct value, (`#<VAL>`) is it is restricted by instruction size 32 bits. (only 8 bit numbers allowed)
    -   So they introduced literal pool. Exists in the same section as that of the code
    -   Allows for you to load address of various "sub-rountines"