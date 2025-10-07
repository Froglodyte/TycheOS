#include "entry.h"
#include "mm.h"
#include "scheduler.h"

int copy_process(unsigned long fn, unsigned long arg) {
    preempt_disable();

    task_struct *p;
    p = (task_struct *)get_free_page();
    if (!p) return 1;

    p->priority = current->priority;
    p->state = TASK_RUNNING;
    p->counter = p->priority;
    p->preempt_count = 1;  // cant't disable this process?

    p->cpu_ctx.x19 = fn;
    p->cpu_ctx.x20 = arg;
    p->cpu_ctx.lr = (unsigned long)ret_from_fork;
    p->cpu_ctx.sp = (unsigned long)p + THREAD_SIZE;
    int pid = curr_task++;
    tasks[pid] = p;
    preempt_enable();
    return 0;
}