/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

<<<<<<< HEAD
#include "linux/kernel.h"
#include "linux/ptrace.h"
#include "kern_util.h"
#include "sysdep/ptrace.h"
#include "sysdep/syscalls.h"
=======
#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <kern_util.h>
#include <sysdep/ptrace.h>
#include <sysdep/syscalls.h>
>>>>>>> refs/remotes/origin/master

extern int syscall_table_size;
#define NR_SYSCALLS (syscall_table_size / sizeof(void *))

void handle_syscall(struct uml_pt_regs *r)
{
	struct pt_regs *regs = container_of(r, struct pt_regs, regs);
	long result;
	int syscall;

<<<<<<< HEAD
	syscall_trace(r, 0);
=======
	syscall_trace_enter(regs);
>>>>>>> refs/remotes/origin/master

	/*
	 * This should go in the declaration of syscall, but when I do that,
	 * strace -f -c bash -c 'ls ; ls' breaks, sometimes not tracing
	 * children at all, sometimes hanging when bash doesn't see the first
	 * ls exit.
	 * The assembly looks functionally the same to me.  This is
	 *     gcc version 4.0.1 20050727 (Red Hat 4.0.1-5)
	 * in case it's a compiler bug.
	 */
	syscall = UPT_SYSCALL_NR(r);
	if ((syscall >= NR_SYSCALLS) || (syscall < 0))
		result = -ENOSYS;
	else result = EXECUTE_SYSCALL(syscall, regs);

<<<<<<< HEAD
	REGS_SET_SYSCALL_RETURN(r->gp, result);

	syscall_trace(r, 1);
=======
	PT_REGS_SET_SYSCALL_RETURN(regs, result);

	syscall_trace_leave(regs);
>>>>>>> refs/remotes/origin/master
}
