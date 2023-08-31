#ifndef MPX_SYSCALL_H
#define MPX_SYSCALL_H

#include <pcb.h>
#include <context.h>

struct context* sys_call(struct context*);

pcb* availablePCB();


#endif
