#include <stdint.h>

#include "fork.h"
#include "fs.h"
#include "irq.h"
#include "mm.h"
#include "printf.h"
#include "scheduler.h"
#include "shell.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

void process(char *array) {
    while (1) {
        for (int i = 0; i < 5; i++) {
            uart_putc(array[i]);
            delay(100000);
        }
    }
}

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
    vfs_init();
    uart_putc('\r');
    uart_putc('\n');
    copy_process((unsigned long)process, (unsigned long)"12345");
    copy_process((unsigned long)process, (unsigned long)"abcde");
    shell();
    // kernel must not terminate
    while (1) {
        schedule();
    }
}