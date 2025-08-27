#include "uart.h"

void shell() {
    uart_puts("TycheOS Shell\r\n> ");

    char cmd[256];
    int idx = 0;

    while (1) {
      char c = uart_getc();
      uart_putc(c);

      if (c == '\r') {
        cmd[idx] = '\0';
        uart_puts("\r\n");

        uart_puts("We'll add in-built cmds soon!\r\n");

        uart_puts("> ");
        idx = 0;
      } else { cmd[idx++] = c; }
    }
}