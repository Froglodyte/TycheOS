#include "timer.h"
#include "uart.h"

// private peripheral timer for the ARM CPU [NOT global]
// address is an offset from the peripheral base
#define CORE0_TIMER_IRQ_CTRL 0x40000040

void timer_init() {
    // read the clock frequency from the system register
    // and set timer value for the next interrupt
    asm volatile("mrs x0, cntfrq_el0; msr cntp_tval_el0, x0");

    // enable the timer
    asm volatile("msr cntp_ctl_el0, %0" : : "r" (1));

    // enable timer interrupts for core 0
    *(unsigned int*)CORE0_TIMER_IRQ_CTRL = 1;
}

void handle_timer_irq() {
    uart_puts(" [tick] ");

    // reset the timer for the next interrupt
    asm volatile("mrs x0, cntfrq_el0; msr cntp_tval_el0, x0");
}
