#include "uart.h"

// added the strcmp function manually lmao
// ill probably move this and a bunch of other utility fns into a separate file later
int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void cmd_help() {
  uart_puts("TycheOS Shell Commands\r\n");
  uart_puts("  [1] help        - Display this help message\r\n");
  uart_puts("  [2] clear       - Clear the terminal screen\r\n");
  uart_puts("  [3] echo [msg]  - Print [msg] back to the terminal\r\n");
  uart_puts("  [4] shutdown    - Shuts your device down\r\n");
  uart_puts("  [5] reboot      - Reboots your device\r\n");

}

void cmd_echo(const char* input) {
  uart_puts(input + 5);
  uart_puts("\r\n");
}

void cmd_clear() {
  uart_puts("\033[2J\033[H");
}

void cmd_shutdown() {
  uart_puts("ill implement this later\r\n");
}

void cmd_reboot() {
  uart_puts("ill implement this later\r\n");
}

void run_cmd(const char* cmd) {
  if (strcmp(cmd, "help") == 0) {
    cmd_help();
  } else if (cmd[0] == 'e' && cmd[1] == 'c'
          && cmd[2] == 'h' && cmd[3] == 'o') {
    cmd_echo(cmd);
  } else if (strcmp(cmd, "shutdown") == 0) {
    cmd_shutdown();
  } else if (strcmp(cmd, "clear") == 0) {
    cmd_clear();
  } else if (strcmp(cmd, "reboot") == 0) {
    cmd_reboot();
  } else {
    uart_puts("Unknown Command :/\r\n");
  }
}

void shell() { 
    uart_puts("TycheOS Shell\r\n> ");

    char cmd[256];
    int idx = 0;

    while (1) {
      char c = uart_getc();
      uart_putc(c);

      // backspace
      if (c == '\b' || c == 127) {
          if (idx > 0) {
              idx--;
              uart_puts("\b \b");
          }
      }

      // enter
      if (c == '\r') {
        cmd[idx] = '\0';
        uart_puts("\r\n");

        run_cmd(cmd);

        uart_puts("> ");
        idx = 0;
      } else { cmd[idx++] = c; }
    }
}