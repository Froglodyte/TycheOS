#include "scheduler.h"
#ifndef FORK
#define FORK

#define PSR_MODE_EL0t 0x00000000

int copy_process(unsigned long clone_flags, unsigned long fn, unsigned long arg,
                 unsigned long stack);
int move_to_user_mode(unsigned long pc);
struct pt_regs *task_pt_regs(task_struct *tsk);

struct pt_regs {
    unsigned long regs[31];
    unsigned long sp;
    unsigned long pc;
    unsigned long pstate;
};

#endif