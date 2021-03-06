/*
 * SA11x0 Assembler Sleep/WakeUp Management Routines
 *
 * Copyright (c) 2001 Cliff Brake <cbrake@accelent.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 * History:
 *
 * 2001-02-06: Cliff Brake         Initial code
 *
 * 2001-08-29:	Nicolas Pitre	Simplified.
 *
 * 2002-05-27:	Nicolas Pitre	Revisited, more cleanup and simplification.
 *				Storage is on the stack now.
 */

#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>

		.text
/*
<<<<<<< HEAD
<<<<<<< HEAD
 * sa1100_cpu_suspend()
 *
 * Causes sa11x0 to enter sleep state
 *
 */

ENTRY(sa1100_cpu_suspend)
	stmfd	sp!, {r4 - r12, lr}		@ save registers on stack
	mov	r1, r0
	ldr	r3, =sa1100_cpu_resume		@ return function
	bl	cpu_suspend

	@ disable clock switching
	mcr	p15, 0, r1, c15, c2, 2

        @ Adjust memory timing before lowering CPU clock
	@ Clock speed adjustment without changing memory timing makes
	@ CPU hang in some cases
        ldr     r0, =MDREFR
        ldr     r1, [r0]
        orr     r1, r1, #MDREFR_K1DB2
        str     r1, [r0]
=======
=======
>>>>>>> refs/remotes/origin/master
 * sa1100_finish_suspend()
 *
 * Causes sa11x0 to enter sleep state
 *
 * Must be aligned to a cacheline.
 */
	.balign	32
ENTRY(sa1100_finish_suspend)
	@ disable clock switching
	mcr	p15, 0, r1, c15, c2, 2

	ldr	r6, =MDREFR
	ldr	r4, [r6]
	orr     r4, r4, #MDREFR_K1DB2
	ldr	r5, =PPCR

<<<<<<< HEAD
	@ Pre-load __udelay into the I-cache
	mov	r0, #1
	bl	__udelay
=======
	@ Pre-load __loop_udelay into the I-cache
	mov	r0, #1
	bl	__loop_udelay
>>>>>>> refs/remotes/origin/master
	mov	r0, r0

	@ The following must all exist in a single cache line to
	@ avoid accessing memory until this sequence is complete,
	@ otherwise we occasionally hang.

	@ Adjust memory timing before lowering CPU clock
	str     r4, [r6]
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	@ delay 90us and set CPU PLL to lowest speed
	@ fixes resume problem on high speed SA1110
	mov	r0, #90
<<<<<<< HEAD
	bl	__udelay
<<<<<<< HEAD
	ldr	r0, =PPCR
	mov	r1, #0
	str	r1, [r0]
=======
	mov	r1, #0
	str	r1, [r5]
>>>>>>> refs/remotes/origin/cm-10.0
	mov	r0, #90
	bl	__udelay
=======
	bl	__loop_udelay
	mov	r1, #0
	str	r1, [r5]
	mov	r0, #90
	bl	__loop_udelay
>>>>>>> refs/remotes/origin/master

	/*
	 * SA1110 SDRAM controller workaround.  register values:
	 *
	 * r0  = &MSC0
	 * r1  = &MSC1
	 * r2  = &MSC2
	 * r3  = MSC0 value
	 * r4  = MSC1 value
	 * r5  = MSC2 value
	 * r6  = &MDREFR
	 * r7  = first MDREFR value
	 * r8  = second MDREFR value
	 * r9  = &MDCNFG
	 * r10 = MDCNFG value
	 * r11 = third MDREFR value
	 * r12 = &PMCR
	 * r13 = PMCR value (1)
	 */

	ldr	r0, =MSC0
	ldr	r1, =MSC1
	ldr	r2, =MSC2

	ldr	r3, [r0]
	bic	r3, r3, #FMsk(MSC_RT)
	bic	r3, r3, #FMsk(MSC_RT)<<16

	ldr	r4, [r1]
	bic	r4, r4, #FMsk(MSC_RT)
	bic	r4, r4, #FMsk(MSC_RT)<<16

	ldr	r5, [r2]
	bic	r5, r5, #FMsk(MSC_RT)
	bic	r5, r5, #FMsk(MSC_RT)<<16

<<<<<<< HEAD
<<<<<<< HEAD
	ldr	r6, =MDREFR

	ldr	r7, [r6]
bic	r7, r7, #0x0000FF00
bic	r7, r7, #0x000000F0
orr	r8, r7, #MDREFR_SLFRSH
=======
=======
>>>>>>> refs/remotes/origin/master
	ldr	r7, [r6]
	bic	r7, r7, #0x0000FF00
	bic	r7, r7, #0x000000F0
	orr	r8, r7, #MDREFR_SLFRSH
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	ldr	r9, =MDCNFG
	ldr	r10, [r9]
	bic	r10, r10, #(MDCNFG_DE0+MDCNFG_DE1)
	bic	r10, r10, #(MDCNFG_DE2+MDCNFG_DE3)

	bic	r11, r8, #MDREFR_SLFRSH
	bic	r11, r11, #MDREFR_E1PIN

	ldr	r12, =PMCR

	mov	r13, #PMCR_SF

	b	sa1110_sdram_controller_fix

	.align 5
sa1110_sdram_controller_fix:

	@ Step 1 clear RT field of all MSCx registers
	str 	r3, [r0]
	str	r4, [r1]
	str	r5, [r2]

	@ Step 2 clear DRI field in MDREFR
	str	r7, [r6]

	@ Step 3 set SLFRSH bit in MDREFR
	str	r8, [r6]

	@ Step 4 clear DE bis in MDCNFG
	str	r10, [r9]

	@ Step 5 clear DRAM refresh control register
	str	r11, [r6]

	@ Wow, now the hardware suspend request pins can be used, that makes them functional for
	@ about 7 ns out of the	entire time that the CPU is running!

	@ Step 6 set force sleep bit in PMCR

	str	r13, [r12]

20:	b	20b			@ loop waiting for sleep
<<<<<<< HEAD
<<<<<<< HEAD

/*
 * cpu_sa1100_resume()
 *
 * entry point from bootloader into kernel during resume
 */
	.align 5
sa1100_cpu_resume:
	mcr	p15, 0, r1, c15, c1, 2		@ enable clock switching
	ldmfd	sp!, {r4 - r12, pc}		@ return to caller
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
