#ifndef GPIO_H
#define GPIO_H
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

#endif