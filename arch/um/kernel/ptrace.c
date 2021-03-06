/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

<<<<<<< HEAD
#include "linux/audit.h"
#include "linux/ptrace.h"
#include "linux/sched.h"
#include "asm/uaccess.h"
#include "skas_ptrace.h"
=======
#include <linux/audit.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/tracehook.h>
#include <asm/uaccess.h>
#include <skas_ptrace.h>
>>>>>>> refs/remotes/origin/master



void user_enable_single_step(struct task_struct *child)
{
	child->ptrace |= PT_DTRACE;
	child->thread.singlestep_syscall = 0;

#ifdef SUBARCH_SET_SINGLESTEPPING
	SUBARCH_SET_SINGLESTEPPING(child, 1);
#endif
}

void user_disable_single_step(struct task_struct *child)
{
	child->ptrace &= ~PT_DTRACE;
	child->thread.singlestep_syscall = 0;

#ifdef SUBARCH_SET_SINGLESTEPPING
	SUBARCH_SET_SINGLESTEPPING(child, 0);
#endif
}

/*
 * Called by kernel/ptrace.c when detaching..
 */
void ptrace_disable(struct task_struct *child)
{
	user_disable_single_step(child);
}

extern int peek_user(struct task_struct * child, long addr, long data);
extern int poke_user(struct task_struct * child, long addr, long data);

long arch_ptrace(struct task_struct *child, long request,
		 unsigned long addr, unsigned long data)
{
	int i, ret;
	unsigned long __user *p = (void __user *)data;
	void __user *vp = p;

	switch (request) {
<<<<<<< HEAD
<<<<<<< HEAD
	/* read word at location addr. */
	case PTRACE_PEEKTEXT:
	case PTRACE_PEEKDATA:
		ret = generic_ptrace_peekdata(child, addr, data);
		break;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* read the word at location addr in the USER area. */
	case PTRACE_PEEKUSR:
		ret = peek_user(child, addr, data);
		break;

<<<<<<< HEAD
<<<<<<< HEAD
	/* write the word at location addr. */
	case PTRACE_POKETEXT:
	case PTRACE_POKEDATA:
		ret = generic_ptrace_pokedata(child, addr, data);
		break;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* write the word at location addr in the USER area */
	case PTRACE_POKEUSR:
		ret = poke_user(child, addr, data);
		break;

	case PTRACE_SYSEMU:
	case PTRACE_SYSEMU_SINGLESTEP:
		ret = -EIO;
		break;

#ifdef PTRACE_GETREGS
	case PTRACE_GETREGS: { /* Get all gp regs from the child. */
		if (!access_ok(VERIFY_WRITE, p, MAX_REG_OFFSET)) {
			ret = -EIO;
			break;
		}
		for ( i = 0; i < MAX_REG_OFFSET; i += sizeof(long) ) {
			__put_user(getreg(child, i), p);
			p++;
		}
		ret = 0;
		break;
	}
#endif
#ifdef PTRACE_SETREGS
	case PTRACE_SETREGS: { /* Set all gp regs in the child. */
		unsigned long tmp = 0;
		if (!access_ok(VERIFY_READ, p, MAX_REG_OFFSET)) {
			ret = -EIO;
			break;
		}
		for ( i = 0; i < MAX_REG_OFFSET; i += sizeof(long) ) {
			__get_user(tmp, p);
			putreg(child, i, tmp);
			p++;
		}
		ret = 0;
		break;
	}
#endif
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef PTRACE_GETFPREGS
	case PTRACE_GETFPREGS: /* Get the child FPU state. */
		ret = get_fpregs(vp, child);
		break;
#endif
#ifdef PTRACE_SETFPREGS
	case PTRACE_SETFPREGS: /* Set the child FPU state. */
		ret = set_fpregs(vp, child);
		break;
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case PTRACE_GET_THREAD_AREA:
		ret = ptrace_get_thread_area(child, addr, vp);
		break;

	case PTRACE_SET_THREAD_AREA:
		ret = ptrace_set_thread_area(child, addr, vp);
		break;

	case PTRACE_FAULTINFO: {
		/*
		 * Take the info from thread->arch->faultinfo,
		 * but transfer max. sizeof(struct ptrace_faultinfo).
		 * On i386, ptrace_faultinfo is smaller!
		 */
		ret = copy_to_user(p, &child->thread.arch.faultinfo,
				   sizeof(struct ptrace_faultinfo)) ?
			-EIO : 0;
		break;
	}

#ifdef PTRACE_LDT
	case PTRACE_LDT: {
		struct ptrace_ldt ldt;

		if (copy_from_user(&ldt, p, sizeof(ldt))) {
			ret = -EIO;
			break;
		}

		/*
		 * This one is confusing, so just punt and return -EIO for
		 * now
		 */
		ret = -EIO;
		break;
	}
#endif
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef PTRACE_ARCH_PRCTL
	case PTRACE_ARCH_PRCTL:
		/* XXX Calls ptrace on the host - needs some SMP thinking */
		ret = arch_prctl(child, data, (void __user *) addr);
		break;
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	default:
		ret = ptrace_request(child, request, addr, data);
		if (ret == -EIO)
			ret = subarch_ptrace(child, request, addr, data);
		break;
	}

	return ret;
}

static void send_sigtrap(struct task_struct *tsk, struct uml_pt_regs *regs,
		  int error_code)
{
	struct siginfo info;

	memset(&info, 0, sizeof(info));
	info.si_signo = SIGTRAP;
	info.si_code = TRAP_BRKPT;

	/* User-mode eip? */
	info.si_addr = UPT_IS_USER(regs) ? (void __user *) UPT_IP(regs) : NULL;

	/* Send us the fake SIGTRAP */
	force_sig_info(SIGTRAP, &info, tsk);
}

/*
 * XXX Check PT_DTRACE vs TIF_SINGLESTEP for singlestepping check and
 * PT_PTRACED vs TIF_SYSCALL_TRACE for syscall tracing check
 */
<<<<<<< HEAD
void syscall_trace(struct uml_pt_regs *regs, int entryexit)
{
	int is_singlestep = (current->ptrace & PT_DTRACE) && entryexit;
	int tracesysgood;

<<<<<<< HEAD
	if (unlikely(current->audit_context)) {
		if (!entryexit)
			audit_syscall_entry(HOST_AUDIT_ARCH,
					    UPT_SYSCALL_NR(regs),
					    UPT_SYSCALL_ARG1(regs),
					    UPT_SYSCALL_ARG2(regs),
					    UPT_SYSCALL_ARG3(regs),
					    UPT_SYSCALL_ARG4(regs));
		else audit_syscall_exit(AUDITSC_RESULT(UPT_SYSCALL_RET(regs)),
					UPT_SYSCALL_RET(regs));
	}
=======
	if (!entryexit)
		audit_syscall_entry(HOST_AUDIT_ARCH,
				    UPT_SYSCALL_NR(regs),
				    UPT_SYSCALL_ARG1(regs),
				    UPT_SYSCALL_ARG2(regs),
				    UPT_SYSCALL_ARG3(regs),
				    UPT_SYSCALL_ARG4(regs));
	else
		audit_syscall_exit(regs);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Fake a debug trap */
	if (is_singlestep)
		send_sigtrap(current, regs, 0);
=======
void syscall_trace_enter(struct pt_regs *regs)
{
	audit_syscall_entry(HOST_AUDIT_ARCH,
			    UPT_SYSCALL_NR(&regs->regs),
			    UPT_SYSCALL_ARG1(&regs->regs),
			    UPT_SYSCALL_ARG2(&regs->regs),
			    UPT_SYSCALL_ARG3(&regs->regs),
			    UPT_SYSCALL_ARG4(&regs->regs));
>>>>>>> refs/remotes/origin/master

	if (!test_thread_flag(TIF_SYSCALL_TRACE))
		return;

<<<<<<< HEAD
	if (!(current->ptrace & PT_PTRACED))
		return;

	/*
	 * the 0x80 provides a way for the tracing parent to distinguish
	 * between a syscall stop and SIGTRAP delivery
	 */
	tracesysgood = (current->ptrace & PT_TRACESYSGOOD);
	ptrace_notify(SIGTRAP | (tracesysgood ? 0x80 : 0));

	if (entryexit) /* force do_signal() --> is_syscall() */
		set_thread_flag(TIF_SIGPENDING);

	/*
	 * this isn't the same as continuing with a signal, but it will do
	 * for normal use.  strace only continues with a signal if the
	 * stopping signal is not SIGTRAP.  -brl
	 */
	if (current->exit_code) {
		send_sig(current->exit_code, current, 1);
		current->exit_code = 0;
	}
=======
	tracehook_report_syscall_entry(regs);
}

void syscall_trace_leave(struct pt_regs *regs)
{
	int ptraced = current->ptrace;

	audit_syscall_exit(regs);

	/* Fake a debug trap */
	if (ptraced & PT_DTRACE)
		send_sigtrap(current, &regs->regs, 0);

	if (!test_thread_flag(TIF_SYSCALL_TRACE))
		return;

	tracehook_report_syscall_exit(regs, 0);
	/* force do_signal() --> is_syscall() */
	if (ptraced & PT_PTRACED)
		set_thread_flag(TIF_SIGPENDING);
>>>>>>> refs/remotes/origin/master
}
