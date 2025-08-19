#include <stdint.h>
#include "uart.h"

void main() {
    uart_init();
    uart_puts("WAZZZUHHHHHH\r\n");

    // kernel must not terminate
    while (1) {}
}
