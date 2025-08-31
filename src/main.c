#include <stdint.h>
#include "uart.h"
#include "shell.h"
#include "mm.h"

void main() {
    uart_init();
    memory_init();
    uart_putc('\r\n');
    shell();

    // kernel must not terminate
    while (1) {}
}