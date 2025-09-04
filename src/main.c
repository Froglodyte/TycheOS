#include <stdint.h>
#include "printf.h"
#include "uart.h"
#include "shell.h"
#include "mm.h"

void main()
{
    uart_init();
    init_printf(0, putc);
    printf("Hello World \n");
    memory_init();
    uart_putc('\r\n');
    shell();

    // kernel must not terminate
    while (1)
    {
    }
}