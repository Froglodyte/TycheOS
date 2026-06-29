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

const int queue_timeslices[MLFQ_QUEUES] = {2, 4, 8};
static unsigned long global_ticks = 0;

void _schedule(void) {
    preempt_disable();

    int current_idx = 0;
    for (int i = 0; i < NR_TASKS; ++i) {
        if (tasks[i] == current) {
            current_idx = i;
            break;
        }
    }

    struct task_struct* winner = NULL;
    for (int q = 0; q < MLFQ_QUEUES; ++q) {
        // Search in a round-robin fashion starting from the next task
        for (int i = 1; i <= NR_TASKS; ++i) {
            int idx = (current_idx + i) % NR_TASKS;
            struct task_struct* p = tasks[idx];
            if (p && p->state == TASK_RUNNING && p->queue_level == q) {
                winner = p;
                break;
            }
        }
        if (winner) {
            break;
        }
    }

    if (!winner) {
        winner = current;
    }

    if (winner->counter <= 0) {
        winner->counter = queue_timeslices[winner->queue_level];
    }

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
    global_ticks++;
    if (global_ticks >= BOOST_INTERVAL) {
        global_ticks = 0;
        // Boost all processes to the highest priority queue
        for (int i = 0; i < NR_TASKS; ++i) {
            if (tasks[i]) {
                tasks[i]->queue_level = 0;
                tasks[i]->counter = queue_timeslices[0];
            }
        }
    }

    current->counter--;

    if (current->counter <= 0) {
        // Demote current task if timeslice is exhausted
        if (current->queue_level < MLFQ_QUEUES - 1) {
            current->queue_level++;
        }
        current->counter = queue_timeslices[current->queue_level];

        if (current->preempt_count > 0) {
            return;
        }

        enable_irq();
        _schedule();
        disable_irq();
    } else if (current->preempt_count == 0) {
        // Check if a higher priority task is ready to preempt the current task
        int preempt = 0;
        for (int i = 0; i < NR_TASKS; ++i) {
            struct task_struct* p = tasks[i];
            if (p && p->state == TASK_RUNNING && p->queue_level < current->queue_level) {
                preempt = 1;
                break;
            }
        }
        if (preempt) {
            enable_irq();
            _schedule();
            disable_irq();
        }
    }
}
