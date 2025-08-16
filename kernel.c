#include <stdint.h>

// peripheral base
#define PERIPHERAL_BASE 0x3F000000

// GPIO registers
#define GPFSEL1    ((volatile uint32_t*)(PERIPHERAL_BASE + 0x00200004))
#define GPPUD      ((volatile uint32_t*)(PERIPHERAL_BASE + 0x00200094))
#define GPPUDCLK0  ((volatile uint32_t*)(PERIPHERAL_BASE + 0x00200098))

// UART registers
#define UART0_BASE (PERIPHERAL_BASE + 0x00201000)
#define UART0_DR   ((volatile uint32_t*)(UART0_BASE + 0x00))
#define UART0_FR   ((volatile uint32_t*)(UART0_BASE + 0x18))
#define UART0_IBRD ((volatile uint32_t*)(UART0_BASE + 0x24))
#define UART0_FBRD ((volatile uint32_t*)(UART0_BASE + 0x28))
#define UART0_LCRH ((volatile uint32_t*)(UART0_BASE + 0x2C))
#define UART0_CR   ((volatile uint32_t*)(UART0_BASE + 0x30))
#define UART0_ICR  ((volatile uint32_t*)(UART0_BASE + 0x44))

void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
         : "=r"(count): [count]"0"(count) : "cc");
}

void uart_init() {
    *UART0_CR = 0; // disables UART

    // configure GPIO pins 14 and 15 to be UART pins (ALT0)
    uint32_t selector = *GPFSEL1;
    selector &= ~((7 << 12) | (7 << 15)); //  clear bits 12-14 and 15-17
    selector |= (4 << 12) | (4 << 15);
    *GPFSEL1 = selector;

    // disable pullup/down for pins 14 and 15
    // this is some weird rpi3 thing and i dont understand it fully either lol
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

void uart_puts(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        uart_putc((unsigned char)str[i]);
    }
}

void main() {
    uart_init();
    uart_puts("WAZZZUHHHHHH\r\n");

    // kernel must not terminate
    while (1) {}
}
