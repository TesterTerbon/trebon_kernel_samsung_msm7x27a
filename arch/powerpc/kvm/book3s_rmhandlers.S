/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Copyright SUSE Linux Products GmbH 2009
 *
 * Authors: Alexander Graf <agraf@suse.de>
 */

#include <asm/ppc_asm.h>
#include <asm/kvm_asm.h>
#include <asm/reg.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/mmu.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/mmu.h>
>>>>>>> refs/remotes/origin/master
#include <asm/page.h>
#include <asm/asm-offsets.h>

#ifdef CONFIG_PPC_BOOK3S_64
#include <asm/exception-64s.h>
#endif

/*****************************************************************************
 *                                                                           *
 *        Real Mode handlers that need to be in low physical memory          *
 *                                                                           *
 ****************************************************************************/

#if defined(CONFIG_PPC_BOOK3S_64)

<<<<<<< HEAD
<<<<<<< HEAD
#define LOAD_SHADOW_VCPU(reg)	GET_PACA(reg)					
#define SHADOW_VCPU_OFF		PACA_KVM_SVCPU
#define MSR_NOIRQ		MSR_KERNEL & ~(MSR_IR | MSR_DR)
=======
>>>>>>> refs/remotes/origin/master
#define FUNC(name) 		GLUE(.,name)

#elif defined(CONFIG_PPC_BOOK3S_32)

<<<<<<< HEAD
#define LOAD_SHADOW_VCPU(reg)						\
	mfspr	reg, SPRN_SPRG_THREAD;					\
	lwz	reg, THREAD_KVM_SVCPU(reg);				\
	/* PPC32 can have a NULL pointer - let's check for that */	\
	mtspr   SPRN_SPRG_SCRATCH1, r12;	/* Save r12 */		\
	mfcr	r12;							\
	cmpwi	reg, 0;							\
	bne	1f;							\
	mfspr	reg, SPRN_SPRG_SCRATCH0;				\
	mtcr	r12;							\
	mfspr	r12, SPRN_SPRG_SCRATCH1;				\
	b	kvmppc_resume_\intno;					\
1:;									\
	mtcr	r12;							\
	mfspr	r12, SPRN_SPRG_SCRATCH1;				\
	tophys(reg, reg)

#define SHADOW_VCPU_OFF		0
#define MSR_NOIRQ		MSR_KERNEL
#define FUNC(name)		name

#endif
=======
#define FUNC(name) 		GLUE(.,name)
#define MTMSR_EERI(reg)		mtmsrd	(reg),1

	.globl	kvmppc_skip_interrupt
kvmppc_skip_interrupt:
	/*
	 * Here all GPRs are unchanged from when the interrupt happened
	 * except for r13, which is saved in SPRG_SCRATCH0.
	 */
	mfspr	r13, SPRN_SRR0
	addi	r13, r13, 4
	mtspr	SPRN_SRR0, r13
	GET_SCRATCH0(r13)
	rfid
	b	.

	.globl	kvmppc_skip_Hinterrupt
kvmppc_skip_Hinterrupt:
	/*
	 * Here all GPRs are unchanged from when the interrupt happened
	 * except for r13, which is saved in SPRG_SCRATCH0.
	 */
	mfspr	r13, SPRN_HSRR0
	addi	r13, r13, 4
	mtspr	SPRN_HSRR0, r13
	GET_SCRATCH0(r13)
	hrfid
	b	.

#elif defined(CONFIG_PPC_BOOK3S_32)

#define FUNC(name)		name
#define MTMSR_EERI(reg)		mtmsr	(reg)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define FUNC(name)		name
>>>>>>> refs/remotes/origin/master

.macro INTERRUPT_TRAMPOLINE intno

.global kvmppc_trampoline_\intno
kvmppc_trampoline_\intno:

<<<<<<< HEAD
<<<<<<< HEAD
	SET_SCRATCH0(r13)		/* Save r13 */
=======
	mtspr	SPRN_SPRG_SCRATCH0, r13		/* Save r13 */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mtspr	SPRN_SPRG_SCRATCH0, r13		/* Save r13 */
>>>>>>> refs/remotes/origin/master

	/*
	 * First thing to do is to find out if we're coming
	 * from a KVM guest or a Linux process.
	 *
	 * To distinguish, we check a magic byte in the PACA/current
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	LOAD_SHADOW_VCPU(r13)
	PPC_STL	r12, (SHADOW_VCPU_OFF + SVCPU_SCRATCH0)(r13)
	mfcr	r12
	stw	r12, (SHADOW_VCPU_OFF + SVCPU_SCRATCH1)(r13)
	lbz	r12, (SHADOW_VCPU_OFF + SVCPU_IN_GUEST)(r13)
	cmpwi	r12, KVM_GUEST_MODE_NONE
	bne	..kvmppc_handler_hasmagic_\intno
	/* No KVM guest? Then jump back to the Linux handler! */
	lwz	r12, (SHADOW_VCPU_OFF + SVCPU_SCRATCH1)(r13)
	mtcr	r12
	PPC_LL	r12, (SHADOW_VCPU_OFF + SVCPU_SCRATCH0)(r13)
	GET_SCRATCH0(r13)			/* r13 = original r13 */
	b	kvmppc_resume_\intno		/* Get back original handler */
=======
=======
>>>>>>> refs/remotes/origin/master
	mfspr	r13, SPRN_SPRG_THREAD
	lwz	r13, THREAD_KVM_SVCPU(r13)
	/* PPC32 can have a NULL pointer - let's check for that */
	mtspr   SPRN_SPRG_SCRATCH1, r12		/* Save r12 */
	mfcr	r12
	cmpwi	r13, 0
	bne	1f
2:	mtcr	r12
	mfspr	r12, SPRN_SPRG_SCRATCH1
	mfspr	r13, SPRN_SPRG_SCRATCH0		/* r13 = original r13 */
	b	kvmppc_resume_\intno		/* Get back original handler */

1:	tophys(r13, r13)
	stw	r12, HSTATE_SCRATCH1(r13)
	mfspr	r12, SPRN_SPRG_SCRATCH1
	stw	r12, HSTATE_SCRATCH0(r13)
	lbz	r12, HSTATE_IN_GUEST(r13)
	cmpwi	r12, KVM_GUEST_MODE_NONE
	bne	..kvmppc_handler_hasmagic_\intno
	/* No KVM guest? Then jump back to the Linux handler! */
	lwz	r12, HSTATE_SCRATCH1(r13)
	b	2b
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Now we know we're handling a KVM guest */
..kvmppc_handler_hasmagic_\intno:

	/* Should we just skip the faulting instruction? */
	cmpwi	r12, KVM_GUEST_MODE_SKIP
	beq	kvmppc_handler_skip_ins

	/* Let's store which interrupt we're handling */
	li	r12, \intno

	/* Jump into the SLB exit code that goes to the highmem handler */
	b	kvmppc_handler_trampoline_exit

.endm

INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_SYSTEM_RESET
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_MACHINE_CHECK
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_DATA_STORAGE
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_INST_STORAGE
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_EXTERNAL
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PPC_BOOK3S_64
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_EXTERNAL_HV
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_ALIGNMENT
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_PROGRAM
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_FP_UNAVAIL
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_DECREMENTER
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_SYSCALL
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_TRACE
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_PERFMON
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_ALTIVEC

<<<<<<< HEAD
<<<<<<< HEAD
/* Those are only available on 64 bit machines */

#ifdef CONFIG_PPC_BOOK3S_64
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_DATA_SEGMENT
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_INST_SEGMENT
INTERRUPT_TRAMPOLINE	BOOK3S_INTERRUPT_VSX
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Bring us back to the faulting code, but skip the
 * faulting instruction.
 *
 * This is a generic exit path from the interrupt
 * trampolines above.
 *
 * Input Registers:
 *
 * R12            = free
 * R13            = Shadow VCPU (PACA)
<<<<<<< HEAD
<<<<<<< HEAD
 * SVCPU.SCRATCH0 = guest R12
 * SVCPU.SCRATCH1 = guest CR
=======
 * HSTATE.SCRATCH0 = guest R12
 * HSTATE.SCRATCH1 = guest CR
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * HSTATE.SCRATCH0 = guest R12
 * HSTATE.SCRATCH1 = guest CR
>>>>>>> refs/remotes/origin/master
 * SPRG_SCRATCH0  = guest R13
 *
 */
kvmppc_handler_skip_ins:

	/* Patch the IP to the next instruction */
	mfsrr0	r12
	addi	r12, r12, 4
	mtsrr0	r12

	/* Clean up all state */
<<<<<<< HEAD
<<<<<<< HEAD
	lwz	r12, (SHADOW_VCPU_OFF + SVCPU_SCRATCH1)(r13)
	mtcr	r12
	PPC_LL	r12, (SHADOW_VCPU_OFF + SVCPU_SCRATCH0)(r13)
=======
	lwz	r12, HSTATE_SCRATCH1(r13)
	mtcr	r12
	PPC_LL	r12, HSTATE_SCRATCH0(r13)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	lwz	r12, HSTATE_SCRATCH1(r13)
	mtcr	r12
	PPC_LL	r12, HSTATE_SCRATCH0(r13)
>>>>>>> refs/remotes/origin/master
	GET_SCRATCH0(r13)

	/* And get back into the code */
	RFI
<<<<<<< HEAD
<<<<<<< HEAD

/*
 * This trampoline brings us back to a real mode handler
 *
 * Input Registers:
 *
 * R5 = SRR0
 * R6 = SRR1
 * LR = real-mode IP
 *
 */
.global kvmppc_handler_lowmem_trampoline
kvmppc_handler_lowmem_trampoline:

	mtsrr0	r5
	mtsrr1	r6
	blr
kvmppc_handler_lowmem_trampoline_end:

/*
 * Call a function in real mode
 *
 * Input Registers:
 *
 * R3 = function
 * R4 = MSR
 * R5 = scratch register
 *
 */
_GLOBAL(kvmppc_rmcall)
	LOAD_REG_IMMEDIATE(r5, MSR_NOIRQ)
	mtmsr	r5		/* Disable relocation and interrupts, so mtsrr
				   doesn't get interrupted */
	sync
	mtsrr0	r3
	mtsrr1	r4
=======
=======
>>>>>>> refs/remotes/origin/master
#endif

/*
 * Call kvmppc_handler_trampoline_enter in real mode
 *
 * On entry, r4 contains the guest shadow MSR
<<<<<<< HEAD
=======
 * MSR.EE has to be 0 when calling this function
>>>>>>> refs/remotes/origin/master
 */
_GLOBAL(kvmppc_entry_trampoline)
	mfmsr	r5
	LOAD_REG_ADDR(r7, kvmppc_handler_trampoline_enter)
	toreal(r7)

<<<<<<< HEAD
	li	r9, MSR_RI
	ori	r9, r9, MSR_EE
	andc	r9, r5, r9	/* Clear EE and RI in MSR value */
	li	r6, MSR_IR | MSR_DR
	ori	r6, r6, MSR_EE
	andc	r6, r5, r6	/* Clear EE, DR and IR in MSR value */
	MTMSR_EERI(r9)		/* Clear EE and RI in MSR */
	mtsrr0	r7		/* before we set srr0/1 */
	mtsrr1	r6
>>>>>>> refs/remotes/origin/cm-10.0
=======
	li	r6, MSR_IR | MSR_DR
	andc	r6, r5, r6	/* Clear DR and IR in MSR value */
	/*
	 * Set EE in HOST_MSR so that it's enabled when we get into our
	 * C exit handler function.
	 */
	ori	r5, r5, MSR_EE
	mtsrr0	r7
	mtsrr1	r6
>>>>>>> refs/remotes/origin/master
	RFI

#if defined(CONFIG_PPC_BOOK3S_32)
#define STACK_LR	INT_FRAME_SIZE+4

/* load_up_xxx have to run with MSR_DR=0 on Book3S_32 */
#define MSR_EXT_START						\
	PPC_STL	r20, _NIP(r1);					\
	mfmsr	r20;						\
	LOAD_REG_IMMEDIATE(r3, MSR_DR|MSR_EE);			\
	andc	r3,r20,r3;		/* Disable DR,EE */	\
	mtmsr	r3;						\
	sync

#define MSR_EXT_END						\
	mtmsr	r20;			/* Enable DR,EE */	\
	sync;							\
	PPC_LL	r20, _NIP(r1)

#elif defined(CONFIG_PPC_BOOK3S_64)
#define STACK_LR	_LINK
#define MSR_EXT_START
#define MSR_EXT_END
#endif

/*
 * Activate current's external feature (FPU/Altivec/VSX)
 */
#define define_load_up(what) 					\
								\
_GLOBAL(kvmppc_load_up_ ## what);				\
	PPC_STLU r1, -INT_FRAME_SIZE(r1);			\
	mflr	r3;						\
	PPC_STL	r3, STACK_LR(r1);				\
	MSR_EXT_START;						\
								\
	bl	FUNC(load_up_ ## what);				\
								\
	MSR_EXT_END;						\
	PPC_LL	r3, STACK_LR(r1);				\
	mtlr	r3;						\
	addi	r1, r1, INT_FRAME_SIZE;				\
	blr

define_load_up(fpu)
#ifdef CONFIG_ALTIVEC
define_load_up(altivec)
#endif
<<<<<<< HEAD
#ifdef CONFIG_VSX
define_load_up(vsx)
#endif

<<<<<<< HEAD
.global kvmppc_trampoline_lowmem
kvmppc_trampoline_lowmem:
	PPC_LONG kvmppc_handler_lowmem_trampoline - CONFIG_KERNEL_START

.global kvmppc_trampoline_enter
kvmppc_trampoline_enter:
	PPC_LONG kvmppc_handler_trampoline_enter - CONFIG_KERNEL_START

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
#include "book3s_segment.S"
