#include <stdint.h>
#include "uart.h"
#include "shell.h"

void main() {
    uart_init();
    shell();

    // kernel must not terminate
    while (1) {}
}