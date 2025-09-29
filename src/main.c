#include <stdint.h>

#include "irq.h"
#include "mm.h"
#include "printf.h"
#include "shell.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

void main() {
    uart_init();
    init_printf(0, putc);
    int el_code = get_el();
    printf("The exception level is %d \n", el_code);
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    enable_irq();
    memory_init();
    uart_putc('\r\n');
    shell();
    // kernel must not terminate
    while (1) {
    }
}