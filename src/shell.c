
#include "shell.h"

#include <stdint.h>

#include "uart.h"

#include "string.h"

static void cmd_help() {
    uart_puts("TycheOS Shell Commands\r\n");
    uart_puts("  help        - Display this help message\r\n");
    uart_puts("  clear       - Clear the terminal screen\r\n");
    uart_puts("  echo [msg]  - Print [msg] back to the terminal\r\n");
    uart_puts("  tick        - Show the current system tick count\r\n");
}

static void cmd_echo(const char *input) {
    uart_puts(input + 5);
    uart_puts("\r\n");
}

static void cmd_clear() { uart_puts("\x1b[2J\x1b[H"); }

static void cmd_tick() {
    char tick_str[21];  // enough to hold 64-bit integer
    // itoa(timer_ticks, tick_str);
    uart_puts("System ticks: ");
    uart_puts(tick_str);
    uart_puts("\r\n");
}

static void run_cmd(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        cmd_echo(cmd);
    } else if (strcmp(cmd, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(cmd, "tick") == 0) {
        cmd_tick();
    } else {
        uart_puts("Unknown Command: ");
        uart_puts(cmd);
        uart_puts("\r\n");
    }
}

void shell() {
    uart_puts("\r\nTycheOS Shell\r\n");
    uart_puts("> ");

    char cmd_buffer[256];
    int index = 0;

    while (1) {
        char c = uart_getc();

        // backspace
        if (c == '\b' || c == 127) {
            if (index > 0) {
                index--;
                uart_puts("\b \b");
            }
        }
        // enter
        else if (c == '\r') {
            cmd_buffer[index] = '\0';
            uart_puts("\r\n");

            if (index > 0) {
                run_cmd(cmd_buffer);
            }

            uart_puts("> ");
            index = 0;  // reset for the next command
        } else {
            // make sure we don't overflow the buffer
            if (index < sizeof(cmd_buffer) - 1) {
                cmd_buffer[index++] = c;
                uart_putc(c);
            }
        }
    }
}