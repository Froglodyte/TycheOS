#include "scheduler.h"

#include <stdint.h>

#include "irq.h"
#include "printf.h"

task_struct init = INIT_TASK;
task_struct* current = &init;

task_struct* tasks[NR_TASKS] = {
    &init,
};

int curr_task = 1;

void preempt_disable() { current->preempt_count++; }

void preempt_enable() { current->preempt_count--; }

static uint64_t rng_state = 88172645463325252ULL;

static uint64_t rng_next(void) {
    uint64_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng_state = x;
    return x;
}

void _schedule(void) {
    preempt_disable();

    struct task_struct* winner = NULL;
    unsigned long total_tickets = 0UL;

    for (int i = 0; i < NR_TASKS; ++i) {
        struct task_struct* p = tasks[i];
        if (!p)
            continue;
        if (p->state != TASK_RUNNING)
            continue;

        int base = p->priority;
        if (base < 0)
            base = 0;

        unsigned long tickets = (unsigned long)(base + (p->counter & 0xff));
        if (tickets == 0)
            tickets = 1UL;

        total_tickets += tickets;
    }

    if (total_tickets == 0UL) {
        preempt_enable();
        return;
    }

    uint64_t r = rng_next();
    unsigned long win = (unsigned long)(r % total_tickets);

    unsigned long acc = 0UL;
    for (int i = 0; i < NR_TASKS; ++i) {
        struct task_struct* p = tasks[i];
        if (!p)
            continue;
        if (p->state != TASK_RUNNING)
            continue;

        int base = p->priority;
        if (base < 0)
            base = 0;
        unsigned long tickets = (unsigned long)(base + (p->counter & 0xff));
        if (tickets == 0)
            tickets = 1UL;

        acc += tickets;
        if (win < acc) {
            winner = p;
            break;
        }
    }

    if (!winner)
        winner = current;

    switch_to(winner);
    preempt_enable();
}

void schedule(void) {
    current->counter = 0;
    _schedule();
}

void switch_to(struct task_struct* next) {
    if (current == next)
        return;
    struct task_struct* prev = current;
    current = next;
    cpu_switch_to(prev, next);
}

void schedule_tail() { preempt_enable(); }

void timer_tick() {
    current->counter--;

    if (current->counter > 0 || current->preempt_count > 0) {
        return;  // Still has time left or preemption disabled
    }

    current->counter = 0;

    enable_irq();
    _schedule();
    disable_irq();
}
