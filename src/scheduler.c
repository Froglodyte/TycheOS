#include "scheduler.h"

#include "irq.h"
#include "printf.h"

task_struct init = INIT_TASK;
task_struct* current = &init;
task_struct* tasks[NR_TASKS] = {
    &init,
};
int curr_task = 1;

void preempt_disable() { current->preempt_count++; };

void preempt_enable() { current->preempt_count--; };

void _schedule() {
    // write a scheduling function
    // pick a new task
    preempt_disable();
    int next, c;
    struct task_struct* p;
    while (1) {
        c = -1;
        next = 0;
        for (int i = 0; i < NR_TASKS; i++) {
            p = tasks[i];
            if (p && p->state == TASK_RUNNING && p->counter > c) {
                c = p->counter;
                next = i;
            }
        }
        if (c) {
            break;
        }
        for (int i = 0; i < NR_TASKS; i++) {
            p = tasks[i];
            if (p) {
                p->counter = (p->counter >> 1) + p->priority;
            }
        }
    }
    switch_to(tasks[next]);
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
    // check to see if the first one is ready to be changed?
    current->counter--;
    if (current->counter > 0 || current->preempt_count > 0) {
        // we can't reschedule stuff
        return;
    }
    // time to schedule another process
    current->counter = 0;  // IMPORTANT to do before the enable
    enable_irq();
    _schedule();
    disable_irq();
}