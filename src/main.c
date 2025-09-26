#include <stdint.h>

#include "mm.h"
#include "printf.h"
#include "shell.h"
#include "uart.h"

void main() {
    uart_init();
    init_printf(0, putc);
    printf("Hello World \n");
    memory_init();
    uart_putc('\r\n');
    shell();

    // kernel must not terminate
    while (1) {
    }
}