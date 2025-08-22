#ifndef UART_H
#define UART_H

void delay(int32_t ticks);
void uart_init();
void uart_putc(char c);
void uart_puts(char* str);

#endif