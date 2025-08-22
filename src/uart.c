#include <stdint.h>
#include "gpio.h"
#include "uart.h"

void delay(int32_t ticks) {
    asm volatile("__delay_%=: subs %[ticks], %[ticks], #1; bne __delay_%=\n"
         : "=r"(ticks): [ticks]"0"(ticks) : "cc");
}

void uart_init() {
    *UART0_CR = 0; // disables UART

    // configure GPIO pins 14 and 15 to be UART pins (ALT0)
    uint32_t selector = *GPFSEL1;
    selector &= ~((7 << 12) | (7 << 15));
    selector |= (4 << 12) | (4 << 15);
    *GPFSEL1 = selector;

    // disable pullup/down for pins 14 and 15
    // RPi has a weird SoC, and you'll need to set the transistors to GND, MAXV or nothing using cycling
    *GPPUD = 0;
    delay(150);
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);
    *GPPUDCLK0 = 0;

    *UART0_ICR = 0x7FF; // clear all interrupts

    // standard baud rate is 115200
    // for some reason you cant tell the hardware you want a 115200 baud rate directly
    // you'll need to write two separate rates and let it do some fucked up calculations.
    *UART0_IBRD = 1; // baud rate (integer part)
    *UART0_FBRD = 40; // baud rate (fractional part)

    *UART0_LCRH = (1 << 4) | (1 << 5) | (1 << 6); // enable FIFO, 8 bit data
    *UART0_CR = (1 << 0) | (1 << 8) | (1 << 9); // enable UART, TX, RX
}

void uart_putc(char c) {
    // wait for the UART to be ready to transmit
    while (*UART0_FR & (1 << 5)) {}
    *UART0_DR = c;
}

void uart_puts(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        uart_putc((unsigned char)str[i]);
    }
}