/*
 * BF561 coreB bootstrap file
 *
 * Copyright 2007-2009 Analog Devices Inc.
 *               Philippe Gerum <rpm@xenomai.org>
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/blackfin.h>
#include <asm/asm-offsets.h>
#include <asm/trace.h>

/*
 * This code must come first as CoreB is hardcoded (in hardware)
 * to start at the beginning of its L1 instruction memory.
 */
.section .l1.text.head

/* Lay the initial stack into the L1 scratch area of Core B */
#define INITIAL_STACK	(COREB_L1_SCRATCH_START + L1_SCRATCH_LENGTH - 12)

ENTRY(_coreb_trampoline_start)
<<<<<<< HEAD
<<<<<<< HEAD
	/* Set the SYSCFG register */
	R0 = 0x36;
	SYSCFG = R0; /*Enable Cycle Counter and Nesting Of Interrupts(3rd Bit)*/
	R0 = 0;

	/*Clear Out All the data and pointer  Registers*/
	R1 = R0;
	R2 = R0;
	R3 = R0;
	R4 = R0;
	R5 = R0;
	R6 = R0;
	R7 = R0;

	P0 = R0;
	P1 = R0;
	P2 = R0;
	P3 = R0;
	P4 = R0;
	P5 = R0;

	LC0 = r0;
	LC1 = r0;
	L0 = r0;
	L1 = r0;
	L2 = r0;
	L3 = r0;

	/* Clear Out All the DAG Registers*/
	B0 = r0;
	B1 = r0;
	B2 = r0;
	B3 = r0;

	I0 = r0;
	I1 = r0;
	I2 = r0;
	I3 = r0;

	M0 = r0;
	M1 = r0;
	M2 = r0;
	M3 = r0;

	trace_buffer_init(p0,r0);

	/* Turn off the icache */
	p0.l = LO(IMEM_CONTROL);
	p0.h = HI(IMEM_CONTROL);
	R1 = [p0];
	R0 = ~ENICPLB;
	R0 = R0 & R1;

	/* Disabling of CPLBs should be proceeded by a CSYNC */
	CSYNC;
	[p0] = R0;
	SSYNC;

	/* Turn off the dcache */
	p0.l = LO(DMEM_CONTROL);
	p0.h = HI(DMEM_CONTROL);
	R1 = [p0];
	R0 = ~ENDCPLB;
	R0 = R0 & R1;

	/* Disabling of CPLBs should be proceeded by a CSYNC */
	CSYNC;
	[p0] = R0;
	SSYNC;

	/* in case of double faults, save a few things */
	p0.l = _init_retx_coreb;
	p0.h = _init_retx_coreb;
	R0 = RETX;
	[P0] = R0;

=======
=======
>>>>>>> refs/remotes/origin/master
	/* Enable Cycle Counter and Nesting Of Interrupts */
#ifdef CONFIG_BFIN_SCRATCH_REG_CYCLES
	R0 = SYSCFG_SNEN;
#else
	R0 = SYSCFG_SNEN | SYSCFG_CCEN;
#endif
	SYSCFG = R0;

	/* Optimization register tricks: keep a base value in the
	 * reserved P registers so we use the load/store with an
	 * offset syntax.  R0 = [P5 + <constant>];
	 *   P5 - core MMR base
	 *   R6 - 0
	 */
	r6 = 0;
	p5.l = 0;
	p5.h = hi(COREMMR_BASE);

	/* Zero out registers required by Blackfin ABI */

	/* Disable circular buffers */
	L0 = r6;
	L1 = r6;
	L2 = r6;
	L3 = r6;

	/* Disable hardware loops in case we were started by 'go' */
	LC0 = r6;
	LC1 = r6;

	/*
	 * Clear ITEST_COMMAND and DTEST_COMMAND registers,
	 * Leaving these as non-zero can confuse the emulator
	 */
	[p5 + (DTEST_COMMAND - COREMMR_BASE)] = r6;
	[p5 + (ITEST_COMMAND - COREMMR_BASE)] = r6;
	CSYNC;

	trace_buffer_init(p0,r0);

	/* Turn off the icache */
	r1 = [p5 + (IMEM_CONTROL - COREMMR_BASE)];
	BITCLR (r1, ENICPLB_P);
	[p5 + (IMEM_CONTROL - COREMMR_BASE)] = r1;
	SSYNC;

	/* Turn off the dcache */
	r1 = [p5 + (DMEM_CONTROL - COREMMR_BASE)];
	BITCLR (r1, ENDCPLB_P);
	[p5 + (DMEM_CONTROL - COREMMR_BASE)] = r1;
	SSYNC;

	/* in case of double faults, save a few things */
	p1.l = _initial_pda_coreb;
	p1.h = _initial_pda_coreb;
	r4 = RETX;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_DEBUG_DOUBLEFAULT
	/* Only save these if we are storing them,
	 * This happens here, since L1 gets clobbered
	 * below
	 */
	GET_PDA(p0, r0);
<<<<<<< HEAD
<<<<<<< HEAD
	r7 = [p0 + PDA_DF_RETX];
	p1.l = _init_saved_retx_coreb;
	p1.h = _init_saved_retx_coreb;
	[p1] = r7;

	r7 = [p0 + PDA_DF_DCPLB];
	p1.l = _init_saved_dcplb_fault_addr_coreb;
	p1.h = _init_saved_dcplb_fault_addr_coreb;
	[p1] = r7;

	r7 = [p0 + PDA_DF_ICPLB];
	p1.l = _init_saved_icplb_fault_addr_coreb;
	p1.h = _init_saved_icplb_fault_addr_coreb;
	[p1] = r7;

	r7 = [p0 + PDA_DF_SEQSTAT];
	p1.l = _init_saved_seqstat_coreb;
	p1.h = _init_saved_seqstat_coreb;
	[p1] = r7;
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
	r0 = [p0 + PDA_DF_RETX];
	r1 = [p0 + PDA_DF_DCPLB];
	r2 = [p0 + PDA_DF_ICPLB];
	r3 = [p0 + PDA_DF_SEQSTAT];
	[p1 + PDA_INIT_DF_RETX] = r0;
	[p1 + PDA_INIT_DF_DCPLB] = r1;
	[p1 + PDA_INIT_DF_ICPLB] = r2;
	[p1 + PDA_INIT_DF_SEQSTAT] = r3;
#endif
	[p1 + PDA_INIT_RETX] = r4;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Initialize stack pointer */
	sp.l = lo(INITIAL_STACK);
	sp.h = hi(INITIAL_STACK);
	fp = sp;
	usp = sp;

	/* This section keeps the processor in supervisor mode
	 * during core B startup.  Branches to the idle task.
	 */

	/* EVT15 = _real_start */

<<<<<<< HEAD
<<<<<<< HEAD
	p0.l = lo(EVT15);
	p0.h = hi(EVT15);
	p1.l = _coreb_start;
	p1.h = _coreb_start;
	[p0] = p1;
	csync;

	p0.l = lo(IMASK);
	p0.h = hi(IMASK);
	p1.l = IMASK_IVG15;
	p1.h = 0x0;
	[p0] = p1;
	csync;
=======
=======
>>>>>>> refs/remotes/origin/master
	p1.l = _coreb_start;
	p1.h = _coreb_start;
	[p5 + (EVT15 - COREMMR_BASE)] = p1;
	csync;

	r0 = EVT_IVG15 (z);
	sti r0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	raise 15;
	p0.l = .LWAIT_HERE;
	p0.h = .LWAIT_HERE;
	reti = p0;
#if defined(ANOMALY_05000281)
	nop; nop; nop;
#endif
	rti;

.LWAIT_HERE:
	jump .LWAIT_HERE;
ENDPROC(_coreb_trampoline_start)

#ifdef CONFIG_HOTPLUG_CPU
.section ".text"
ENTRY(_coreb_die)
	sp.l = lo(INITIAL_STACK);
	sp.h = hi(INITIAL_STACK);
	fp = sp;
	usp = sp;

	CLI R2;
	SSYNC;
	IDLE;
	STI R2;

	R0 = IWR_DISABLE_ALL;
	P0.H = hi(SYSMMR_BASE);
	P0.L = lo(SYSMMR_BASE);
	[P0 + (SICB_IWR0 - SYSMMR_BASE)] = R0;
	[P0 + (SICB_IWR1 - SYSMMR_BASE)] = R0;
	SSYNC;

	p0.h = hi(COREB_L1_CODE_START);
	p0.l = lo(COREB_L1_CODE_START);
	jump (p0);
ENDPROC(_coreb_die)
#endif

__INIT
ENTRY(_coreb_start)
	[--sp] = reti;

	p0.l = lo(WDOGB_CTL);
	p0.h = hi(WDOGB_CTL);
	r0 = 0xAD6(z);
	w[p0] = r0;	/* Clear the watchdog. */
	ssync;

	/*
	 * switch to IDLE stack.
	 */
	p0.l = _secondary_stack;
	p0.h = _secondary_stack;
	sp = [p0];
	usp = sp;
	fp = sp;
#ifdef CONFIG_HOTPLUG_CPU
	p0.l = _hotplug_coreb;
	p0.h = _hotplug_coreb;
	r0 = [p0];
	cc = BITTST(r0, 0);
	if cc jump 3f;
#endif
	sp += -12;
	call _init_pda
	sp += 12;
#ifdef CONFIG_HOTPLUG_CPU
3:
#endif
	call _secondary_start_kernel;
.L_exit:
	jump.s	.L_exit;
ENDPROC(_coreb_start)
