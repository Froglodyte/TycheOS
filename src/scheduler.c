#include "scheduler.h"

#include "irq.h"

task_struct init = INIT_TASK;
task_struct* current = &init;
task_struct* tasks[NR_TASKS] = {
    &init,
};
int curr_task = 1;

void preempt_disable() { current->preempt_count++; };

void preempty_enable() { current->preempt_count--; };

void schedule() {
    // write a scheduling function
    // pick a new task
    task_struct* next;
    int count = 0;
    while (1) {
        for (int i = 0; i < NR_TASKS; i++) {
            // if (tasks[i] == )
        } 
    }
}

void switch_to(struct task_struct* next) {
    if (current == next) return;
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
    schedule();
    disable_irq();
}