SECTIONS
{
    . = 0x8000; /*RPi 3 starts the kernel at 0x8000 always*/
    
    .text.boot : { *(.text.boot) }
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    
    /*for efficient memory usage*/
    .bss : {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }
}
