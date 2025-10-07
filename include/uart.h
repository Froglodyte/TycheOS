#ifndef UART_H
#define UART_H
#include <stdint.h>

void delay(int32_t ticks);
void uart_init();
void uart_putc(char c);
void uart_puts(const char *str);
char uart_getc();
void putc(void *p, char c);

#endif