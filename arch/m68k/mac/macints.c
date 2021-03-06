/*
 *	Macintosh interrupts
 *
 * General design:
 * In contrary to the Amiga and Atari platforms, the Mac hardware seems to
 * exclusively use the autovector interrupts (the 'generic level0-level7'
 * interrupts with exception vectors 0x19-0x1f). The following interrupt levels
 * are used:
 *	1	- VIA1
 *		  - slot 0: one second interrupt (CA2)
 *		  - slot 1: VBlank (CA1)
 *		  - slot 2: ADB data ready (SR full)
 *		  - slot 3: ADB data  (CB2)
 *		  - slot 4: ADB clock (CB1)
 *		  - slot 5: timer 2
 *		  - slot 6: timer 1
 *		  - slot 7: status of IRQ; signals 'any enabled int.'
 *
 *	2	- VIA2 or RBV
 *		  - slot 0: SCSI DRQ (CA2)
 *		  - slot 1: NUBUS IRQ (CA1) need to read port A to find which
 *		  - slot 2: /EXP IRQ (only on IIci)
 *		  - slot 3: SCSI IRQ (CB2)
 *		  - slot 4: ASC IRQ (CB1)
 *		  - slot 5: timer 2 (not on IIci)
 *		  - slot 6: timer 1 (not on IIci)
 *		  - slot 7: status of IRQ; signals 'any enabled int.'
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *	2	- OSS (IIfx only?)
 *		  - slot 0: SCSI interrupt
 *		  - slot 1: Sound interrupt
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Levels 3-6 vary by machine type. For VIA or RBV Macintoshes:
 *
 *	3	- unused (?)
 *
 *	4	- SCC
 *
 *	5	- unused (?)
 *		  [serial errors or special conditions seem to raise level 6
 *		  interrupts on some models (LC4xx?)]
 *
 *	6	- off switch (?)
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * For OSS Macintoshes (IIfx only at this point):
 *
 *	3	- Nubus interrupt
 *		  - slot 0: Slot $9
 *		  - slot 1: Slot $A
 *		  - slot 2: Slot $B
 *		  - slot 3: Slot $C
 *		  - slot 4: Slot $D
 *		  - slot 5: Slot $E
 *
 *	4	- SCC IOP
 *
 *	5	- ISM IOP (ADB?)
 *
 *	6	- unused
=======
=======
>>>>>>> refs/remotes/origin/master
 * Machines with Quadra-like VIA hardware, except PSC and PMU machines, support
 * an alternate interrupt mapping, as used by A/UX. It spreads ethernet and
 * sound out to their own autovector IRQs and gives VIA1 a higher priority:
 *
 *	1	- unused (?)
 *
 *	3	- on-board SONIC
 *
 *	5	- Apple Sound Chip (ASC)
 *
 *	6	- VIA1
 *
 * For OSS Macintoshes (IIfx only), we apply an interrupt mapping similar to
 * the Quadra (A/UX) mapping:
 *
 *	1	- ISM IOP (ADB)
 *
 *	2	- SCSI
 *
 *	3	- NuBus
 *
 *	4	- SCC IOP
 *
 *	6	- VIA1
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *
 * For PSC Macintoshes (660AV, 840AV):
 *
 *	3	- PSC level 3
 *		  - slot 0: MACE
 *
 *	4	- PSC level 4
 *		  - slot 1: SCC channel A interrupt
 *		  - slot 2: SCC channel B interrupt
 *		  - slot 3: MACE DMA
 *
 *	5	- PSC level 5
 *
 *	6	- PSC level 6
 *
 * Finally we have good 'ole level 7, the non-maskable interrupt:
 *
 *	7	- NMI (programmer's switch on the back of some Macs)
 *		  Also RAM parity error on models which support it (IIc, IIfx?)
 *
 * The current interrupt logic looks something like this:
 *
 * - We install dispatchers for the autovector interrupts (1-7). These
 *   dispatchers are responsible for querying the hardware (the
 *   VIA/RBV/OSS/PSC chips) to determine the actual interrupt source. Using
 *   this information a machspec interrupt number is generated by placing the
 *   index of the interrupt hardware into the low three bits and the original
 *   autovector interrupt number in the upper 5 bits. The handlers for the
 *   resulting machspec interrupt are then called.
 *
 * - Nubus is a special case because its interrupts are hidden behind two
 *   layers of hardware. Nubus interrupts come in as index 1 on VIA #2,
 *   which translates to IRQ number 17. In this spot we install _another_
 *   dispatcher. This dispatcher finds the interrupting slot number (9-F) and
 *   then forms a new machspec interrupt number as above with the slot number
 *   minus 9 in the low three bits and the pseudo-level 7 in the upper five
 *   bits.  The handlers for this new machspec interrupt number are then
 *   called. This puts Nubus interrupts into the range 56-62.
 *
 * - The Baboon interrupts (used on some PowerBooks) are an even more special
 *   case. They're hidden behind the Nubus slot $C interrupt thus adding a
 *   third layer of indirection. Why oh why did the Apple engineers do that?
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * - We support "fast" and "slow" handlers, just like the Amiga port. The
 *   fast handlers are called first and with all interrupts disabled. They
 *   are expected to execute quickly (hence the name). The slow handlers are
 *   called last with interrupts enabled and the interrupt level restored.
 *   They must therefore be reentrant.
 *
 *   TODO:
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/interrupt.h> /* for intr_count */
#include <linux/delay.h>
#include <linux/seq_file.h>

#include <asm/system.h>
#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/bootinfo.h>
#include <asm/macintosh.h>
#include <asm/mac_via.h>
#include <asm/mac_psc.h>
#include <asm/hwtest.h>
#include <asm/errno.h>
#include <asm/macints.h>
#include <asm/irq_regs.h>
#include <asm/mac_oss.h>
=======
=======
>>>>>>> refs/remotes/origin/master
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>

#include <asm/irq.h>
#include <asm/macintosh.h>
#include <asm/macints.h>
#include <asm/mac_via.h>
#include <asm/mac_psc.h>
#include <asm/mac_oss.h>
#include <asm/mac_iop.h>
#include <asm/mac_baboon.h>
#include <asm/hwtest.h>
#include <asm/irq_regs.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#define SHUTUP_SONIC

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * VIA/RBV hooks
 */

extern void via_register_interrupts(void);
extern void via_irq_enable(int);
extern void via_irq_disable(int);
extern void via_irq_clear(int);
extern int  via_irq_pending(int);

/*
 * OSS hooks
 */

extern void oss_register_interrupts(void);
extern void oss_irq_enable(int);
extern void oss_irq_disable(int);
extern void oss_irq_clear(int);
extern int  oss_irq_pending(int);

/*
 * PSC hooks
 */

extern void psc_register_interrupts(void);
extern void psc_irq_enable(int);
extern void psc_irq_disable(int);
extern void psc_irq_clear(int);
extern int  psc_irq_pending(int);

/*
 * IOP hooks
 */

extern void iop_register_interrupts(void);

/*
 * Baboon hooks
 */

extern int baboon_present;

extern void baboon_register_interrupts(void);
extern void baboon_irq_enable(int);
extern void baboon_irq_disable(int);
extern void baboon_irq_clear(int);

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * console_loglevel determines NMI handler function
 */

irqreturn_t mac_nmi_handler(int, void *);
irqreturn_t mac_debug_handler(int, void *);

/* #define DEBUG_MACINTS */

<<<<<<< HEAD
<<<<<<< HEAD
void mac_enable_irq(unsigned int irq);
void mac_disable_irq(unsigned int irq);

static struct irq_controller mac_irq_controller = {
	.name		= "mac",
	.lock		= __SPIN_LOCK_UNLOCKED(mac_irq_controller.lock),
	.enable		= mac_enable_irq,
	.disable	= mac_disable_irq,
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int mac_irq_startup(struct irq_data *);
static void mac_irq_shutdown(struct irq_data *);

static struct irq_chip mac_irq_chip = {
	.name		= "mac",
	.irq_enable	= mac_irq_enable,
	.irq_disable	= mac_irq_disable,
	.irq_startup	= mac_irq_startup,
	.irq_shutdown	= mac_irq_shutdown,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

void __init mac_init_IRQ(void)
{
#ifdef DEBUG_MACINTS
	printk("mac_init_IRQ(): Setting things up...\n");
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	m68k_setup_irq_controller(&mac_irq_controller, IRQ_USER,
=======
	m68k_setup_irq_controller(&mac_irq_chip, handle_simple_irq, IRQ_USER,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	m68k_setup_irq_controller(&mac_irq_chip, handle_simple_irq, IRQ_USER,
>>>>>>> refs/remotes/origin/master
				  NUM_MAC_SOURCES - IRQ_USER);
	/* Make sure the SONIC interrupt is cleared or things get ugly */
#ifdef SHUTUP_SONIC
	printk("Killing onboard sonic... ");
	/* This address should hopefully be mapped already */
	if (hwreg_present((void*)(0x50f0a000))) {
		*(long *)(0x50f0a014) = 0x7fffL;
		*(long *)(0x50f0a010) = 0L;
	}
	printk("Done.\n");
#endif /* SHUTUP_SONIC */

	/*
	 * Now register the handlers for the master IRQ handlers
	 * at levels 1-7. Most of the work is done elsewhere.
	 */

	if (oss_present)
		oss_register_interrupts();
	else
		via_register_interrupts();
	if (psc_present)
		psc_register_interrupts();
	if (baboon_present)
		baboon_register_interrupts();
	iop_register_interrupts();
	if (request_irq(IRQ_AUTO_7, mac_nmi_handler, 0, "NMI",
			mac_nmi_handler))
		pr_err("Couldn't register NMI\n");
#ifdef DEBUG_MACINTS
	printk("mac_init_IRQ(): Done!\n");
#endif
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 *  mac_enable_irq - enable an interrupt source
 * mac_disable_irq - disable an interrupt source
 *   mac_clear_irq - clears a pending interrupt
 * mac_pending_irq - Returns the pending status of an IRQ (nonzero = pending)
=======
 *  mac_irq_enable - enable an interrupt source
 * mac_irq_disable - disable an interrupt source
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  mac_irq_enable - enable an interrupt source
 * mac_irq_disable - disable an interrupt source
>>>>>>> refs/remotes/origin/master
 *
 * These routines are just dispatchers to the VIA/OSS/PSC routines.
 */

<<<<<<< HEAD
<<<<<<< HEAD
void mac_enable_irq(unsigned int irq)
{
=======
void mac_irq_enable(struct irq_data *data)
{
	int irq = data->irq;
>>>>>>> refs/remotes/origin/cm-10.0
=======
void mac_irq_enable(struct irq_data *data)
{
	int irq = data->irq;
>>>>>>> refs/remotes/origin/master
	int irq_src = IRQ_SRC(irq);

	switch(irq_src) {
	case 1:
<<<<<<< HEAD
<<<<<<< HEAD
		via_irq_enable(irq);
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case 2:
	case 7:
		if (oss_present)
			oss_irq_enable(irq);
		else
			via_irq_enable(irq);
		break;
	case 3:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case 4:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case 4:
>>>>>>> refs/remotes/origin/master
	case 5:
	case 6:
		if (psc_present)
			psc_irq_enable(irq);
		else if (oss_present)
			oss_irq_enable(irq);
		break;
<<<<<<< HEAD
<<<<<<< HEAD
	case 4:
		if (psc_present)
			psc_irq_enable(irq);
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case 8:
		if (baboon_present)
			baboon_irq_enable(irq);
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
void mac_disable_irq(unsigned int irq)
{
=======
void mac_irq_disable(struct irq_data *data)
{
	int irq = data->irq;
>>>>>>> refs/remotes/origin/cm-10.0
=======
void mac_irq_disable(struct irq_data *data)
{
	int irq = data->irq;
>>>>>>> refs/remotes/origin/master
	int irq_src = IRQ_SRC(irq);

	switch(irq_src) {
	case 1:
<<<<<<< HEAD
<<<<<<< HEAD
		via_irq_disable(irq);
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case 2:
	case 7:
		if (oss_present)
			oss_irq_disable(irq);
		else
			via_irq_disable(irq);
		break;
	case 3:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case 4:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case 4:
>>>>>>> refs/remotes/origin/master
	case 5:
	case 6:
		if (psc_present)
			psc_irq_disable(irq);
		else if (oss_present)
			oss_irq_disable(irq);
		break;
<<<<<<< HEAD
<<<<<<< HEAD
	case 4:
		if (psc_present)
			psc_irq_disable(irq);
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case 8:
		if (baboon_present)
			baboon_irq_disable(irq);
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
void mac_clear_irq(unsigned int irq)
{
	switch(IRQ_SRC(irq)) {
	case 1:
		via_irq_clear(irq);
		break;
	case 2:
	case 7:
		if (oss_present)
			oss_irq_clear(irq);
		else
			via_irq_clear(irq);
		break;
	case 3:
	case 5:
	case 6:
		if (psc_present)
			psc_irq_clear(irq);
		else if (oss_present)
			oss_irq_clear(irq);
		break;
	case 4:
		if (psc_present)
			psc_irq_clear(irq);
		break;
	case 8:
		if (baboon_present)
			baboon_irq_clear(irq);
		break;
	}
}

int mac_irq_pending(unsigned int irq)
{
	switch(IRQ_SRC(irq)) {
	case 1:
		return via_irq_pending(irq);
	case 2:
	case 7:
		if (oss_present)
			return oss_irq_pending(irq);
		else
			return via_irq_pending(irq);
	case 3:
	case 5:
	case 6:
		if (psc_present)
			return psc_irq_pending(irq);
		else if (oss_present)
			return oss_irq_pending(irq);
		break;
	case 4:
		if (psc_present)
			psc_irq_pending(irq);
		break;
	}
	return 0;
}
EXPORT_SYMBOL(mac_irq_pending);
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int mac_irq_startup(struct irq_data *data)
{
	int irq = data->irq;

	if (IRQ_SRC(irq) == 7 && !oss_present)
		via_nubus_irq_startup(irq);
	else
		mac_irq_enable(data);

	return 0;
}

static void mac_irq_shutdown(struct irq_data *data)
{
	int irq = data->irq;

	if (IRQ_SRC(irq) == 7 && !oss_present)
		via_nubus_irq_shutdown(irq);
	else
		mac_irq_disable(data);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static int num_debug[8];

irqreturn_t mac_debug_handler(int irq, void *dev_id)
{
	if (num_debug[irq] < 10) {
		printk("DEBUG: Unexpected IRQ %d\n", irq);
		num_debug[irq]++;
	}
	return IRQ_HANDLED;
}

static int in_nmi;
static volatile int nmi_hold;

irqreturn_t mac_nmi_handler(int irq, void *dev_id)
{
	int i;
	/*
	 * generate debug output on NMI switch if 'debug' kernel option given
	 * (only works with Penguin!)
	 */

	in_nmi++;
	for (i=0; i<100; i++)
		udelay(1000);

	if (in_nmi == 1) {
		nmi_hold = 1;
		printk("... pausing, press NMI to resume ...");
	} else {
		printk(" ok!\n");
		nmi_hold = 0;
	}

	barrier();

	while (nmi_hold == 1)
		udelay(1000);

	if (console_loglevel >= 8) {
#if 0
		struct pt_regs *fp = get_irq_regs();
		show_state();
		printk("PC: %08lx\nSR: %04x  SP: %p\n", fp->pc, fp->sr, fp);
		printk("d0: %08lx    d1: %08lx    d2: %08lx    d3: %08lx\n",
		       fp->d0, fp->d1, fp->d2, fp->d3);
		printk("d4: %08lx    d5: %08lx    a0: %08lx    a1: %08lx\n",
		       fp->d4, fp->d5, fp->a0, fp->a1);

		if (STACK_MAGIC != *(unsigned long *)current->kernel_stack_page)
			printk("Corrupted stack page\n");
		printk("Process %s (pid: %d, stackpage=%08lx)\n",
			current->comm, current->pid, current->kernel_stack_page);
		if (intr_count == 1)
			dump_stack((struct frame *)fp);
#else
		/* printk("NMI "); */
#endif
	}
	in_nmi--;
	return IRQ_HANDLED;
}
