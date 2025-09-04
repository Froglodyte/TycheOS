# ARM Assembly Learning

## Data alignment and word size

-   Word size generally refers to the maximum no of bytes a register can hold at once. The memory is aligned in such a fashion that, to store a value, it must be aligned according to the word size boundaries, else it may cause perf issues or may even cause a CPU exception
-   See structure padding and packing in C!
-   Hence data must be aligned in the memory according to the word size. `.align`, `.p2align`, `.balign` directives help us achieve exactly this
-   It is 64 bits for this project!

## Stack Pointer and the Stack

i'll do this later

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
-   ARMv8-A calls these registers (Qx, Vx.<suffix>) as NEON registers.

## AAPCS (Procedure call standard for the ARM 64-bit architecture)

Defines the ABI contract, which is the compiler toolchains follow to produce assembly code. I'll add upon this later. Strongly coupled with "Stack Pointer and Stack"
