#include "fork.h"

#include "entry.h"
#include "mm.h"
#include "printf.h"
#include "scheduler.h"
#include "utils.h"

int copy_process(unsigned long clone_flags, unsigned long fn, unsigned long arg,
                 unsigned long stack) {
    preempt_disable();
    struct task_struct *p;

    p = (struct task_struct *)get_free_page();
    if (!p) {
        return -1;
    }

    struct pt_regs *childregs = task_pt_regs(p);
    memzero((unsigned long)childregs, sizeof(struct pt_regs));
    memzero((unsigned long)&p->cpu_ctx, sizeof(struct cpu_ctx));

    if (clone_flags & PF_KTHREAD) {
        p->cpu_ctx.x19 = fn;
        p->cpu_ctx.x20 = arg;
    } else {
        struct pt_regs *cur_regs = task_pt_regs(current);
        *childregs = *cur_regs;
        childregs->regs[0] = 0;
        childregs->sp = stack + PAGE_SIZE;
        p->stack = stack;
    }
    p->flags = clone_flags;
    p->priority = current->priority;
    p->state = TASK_RUNNING;
    p->queue_level = 0;
    p->counter = queue_timeslices[0];
    p->preempt_count = 1;  // disable preemption until schedule_tail
    p->cwd = current->cwd; // inherit working directory
    for (int i = 0; i < MAX_FILES_PER_PROCESS; i++) {
        p->file_table[i] = 0; // initialize empty FD table
    }

    p->cpu_ctx.lr = (unsigned long)ret_from_fork;
    p->cpu_ctx.sp = (unsigned long)childregs;

    if (curr_task >= NR_TASKS) {
        // Free allocated page and fail
        free_page((unsigned long)p);
        preempt_enable();
        return -1;
    }

    int pid = curr_task++;
    tasks[pid] = p;
    preempt_enable();
    return pid;
}

int move_to_user_mode(unsigned long pc) {
    struct pt_regs *regs = task_pt_regs(current);
    memzero((unsigned long)regs, sizeof(*regs));
    regs->pc = pc;
    regs->pstate = PSR_MODE_EL0t;
    unsigned long stack = get_free_page();  // allocate new user stack
    if (!stack) {
        return -1;
    }
    regs->sp = stack + PAGE_SIZE;
    current->stack = stack;
    return 0;
}

struct pt_regs *task_pt_regs(task_struct *task) {
    unsigned long p = (unsigned long)task + THREAD_SIZE - sizeof(struct pt_regs);
    return (struct pt_regs *)p;
}