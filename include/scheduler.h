#include "mm.h"
#ifndef SCHED_H
#define SCHED_H

#ifndef __ASSEMBLER__

#define THREAD_SIZE PAGE_SIZE  // 4KB
#define NR_TASKS 64
#define TASK_RUNNING 0

// x19 to x28 are callee saved
// x9 to x15 are caller saved
struct cpu_ctx {
    unsigned long x19;
    unsigned long x20;
    unsigned long x21;
    unsigned long x22;
    unsigned long x23;
    unsigned long x24;
    unsigned long x25;
    unsigned long x26;
    unsigned long x27;
    unsigned long x28;
    unsigned long fp;  // x29
    unsigned long lr;  // x30 (Link register)
    unsigned long sp;  // kinda like the XZR register
};

struct task_struct {
    struct cpu_ctx cpu_ctx;
    long state;          // which state the process is in
    long counter;        // how long can this process still run for
    long priority;       // counter = priority
    long preempt_count;  // can this be pre-empted. If non-zero we can't!
};
typedef struct task_struct task_struct;

extern task_struct *current;
extern task_struct *tasks[NR_TASKS];
extern int curr_task;

extern void sched_init(void);
extern void schedule(void);
extern void timer_tick(void);
extern void preempt_disable(void);
extern void preempt_enable(void);
extern void switch_to(task_struct *next);
extern void cpu_switch_to(task_struct *prev, task_struct *next);

#define INIT_TASK \
    /*cpu_context*/ {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* state etc */ 0, 0, 1, 0}

#endif

#endif