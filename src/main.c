#include <stdint.h>

#include "mm.h"
#include "printf.h"
#include "shell.h"
#include "uart.h"
#include "timer.h"

void main() {
    uart_init();
    init_printf(0, putc);
    timer_init();
    //irq_vector_init();
    //enable_interrupt_controller();
    //enable_irq();
    //int el_code = get_el();
    //printf("The elcode is %d \n", el_code);
    memory_init();
    uart_putc('\r\n');
    shell();

    // kernel must not terminate
    while (1) {
    }
}