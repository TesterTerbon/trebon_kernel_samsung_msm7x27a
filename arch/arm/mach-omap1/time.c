/*
 * linux/arch/arm/mach-omap1/time.c
 *
 * OMAP Timers
 *
 * Copyright (C) 2004 Nokia Corporation
 * Partial timer rewrite and additional dynamic tick timer support by
 * Tony Lindgen <tony@atomide.com> and
 * Tuukka Tikkanen <tuukka.tikkanen@elektrobit.com>
 *
 * MPU timer code based on the older MPU timer code for OMAP
 * Copyright (C) 2000 RidgeRun, Inc.
 * Author: Greg Lonnon <glonnon@ridgerun.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sched.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/io.h>
<<<<<<< HEAD

<<<<<<< HEAD
#include <asm/system.h>
#include <mach/hardware.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/leds.h>
#include <asm/irq.h>
#include <asm/sched_clock.h>

<<<<<<< HEAD
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

#include <plat/common.h>
=======
=======
#include <linux/sched_clock.h>

#include <asm/irq.h>

>>>>>>> refs/remotes/origin/master
#include <mach/hardware.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

#include "iomap.h"
#include "common.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_OMAP_MPU_TIMER

#define OMAP_MPU_TIMER_BASE		OMAP_MPU_TIMER1_BASE
#define OMAP_MPU_TIMER_OFFSET		0x100

typedef struct {
	u32 cntl;			/* CNTL_TIMER, R/W */
	u32 load_tim;			/* LOAD_TIM,   W */
	u32 read_tim;			/* READ_TIM,   R */
} omap_mpu_timer_regs_t;

#define omap_mpu_timer_base(n)							\
((omap_mpu_timer_regs_t __iomem *)OMAP1_IO_ADDRESS(OMAP_MPU_TIMER_BASE +	\
				 (n)*OMAP_MPU_TIMER_OFFSET))

static inline unsigned long notrace omap_mpu_timer_read(int nr)
{
	omap_mpu_timer_regs_t __iomem *timer = omap_mpu_timer_base(nr);
	return readl(&timer->read_tim);
}

static inline void omap_mpu_set_autoreset(int nr)
{
	omap_mpu_timer_regs_t __iomem *timer = omap_mpu_timer_base(nr);

	writel(readl(&timer->cntl) | MPU_TIMER_AR, &timer->cntl);
}

static inline void omap_mpu_remove_autoreset(int nr)
{
	omap_mpu_timer_regs_t __iomem *timer = omap_mpu_timer_base(nr);

	writel(readl(&timer->cntl) & ~MPU_TIMER_AR, &timer->cntl);
}

static inline void omap_mpu_timer_start(int nr, unsigned long load_val,
					int autoreset)
{
	omap_mpu_timer_regs_t __iomem *timer = omap_mpu_timer_base(nr);
	unsigned int timerflags = MPU_TIMER_CLOCK_ENABLE | MPU_TIMER_ST;

	if (autoreset)
		timerflags |= MPU_TIMER_AR;

	writel(MPU_TIMER_CLOCK_ENABLE, &timer->cntl);
	udelay(1);
	writel(load_val, &timer->load_tim);
        udelay(1);
	writel(timerflags, &timer->cntl);
}

static inline void omap_mpu_timer_stop(int nr)
{
	omap_mpu_timer_regs_t __iomem *timer = omap_mpu_timer_base(nr);

	writel(readl(&timer->cntl) & ~MPU_TIMER_ST, &timer->cntl);
}

/*
 * ---------------------------------------------------------------------------
 * MPU timer 1 ... count down to zero, interrupt, reload
 * ---------------------------------------------------------------------------
 */
static int omap_mpu_set_next_event(unsigned long cycles,
				   struct clock_event_device *evt)
{
	omap_mpu_timer_start(0, cycles, 0);
	return 0;
}

static void omap_mpu_set_mode(enum clock_event_mode mode,
			      struct clock_event_device *evt)
{
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		omap_mpu_set_autoreset(0);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		omap_mpu_timer_stop(0);
		omap_mpu_remove_autoreset(0);
		break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_RESUME:
		break;
	}
}

static struct clock_event_device clockevent_mpu_timer1 = {
	.name		= "mpu_timer1",
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
<<<<<<< HEAD
	.shift		= 32,
=======
>>>>>>> refs/remotes/origin/master
	.set_next_event	= omap_mpu_set_next_event,
	.set_mode	= omap_mpu_set_mode,
};

static irqreturn_t omap_mpu_timer1_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &clockevent_mpu_timer1;

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct irqaction omap_mpu_timer1_irq = {
	.name		= "mpu_timer1",
<<<<<<< HEAD
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
=======
	.flags		= IRQF_TIMER | IRQF_IRQPOLL,
>>>>>>> refs/remotes/origin/master
	.handler	= omap_mpu_timer1_interrupt,
};

static __init void omap_init_mpu_timer(unsigned long rate)
{
	setup_irq(INT_TIMER1, &omap_mpu_timer1_irq);
	omap_mpu_timer_start(0, (rate / HZ) - 1, 1);

<<<<<<< HEAD
	clockevent_mpu_timer1.mult = div_sc(rate, NSEC_PER_SEC,
					    clockevent_mpu_timer1.shift);
	clockevent_mpu_timer1.max_delta_ns =
		clockevent_delta2ns(-1, &clockevent_mpu_timer1);
	clockevent_mpu_timer1.min_delta_ns =
		clockevent_delta2ns(1, &clockevent_mpu_timer1);

	clockevent_mpu_timer1.cpumask = cpumask_of(0);
	clockevents_register_device(&clockevent_mpu_timer1);
=======
	clockevent_mpu_timer1.cpumask = cpumask_of(0);
	clockevents_config_and_register(&clockevent_mpu_timer1, rate,
					1, -1);
>>>>>>> refs/remotes/origin/master
}


/*
 * ---------------------------------------------------------------------------
 * MPU timer 2 ... free running 32-bit clock source and scheduler clock
 * ---------------------------------------------------------------------------
 */

<<<<<<< HEAD
<<<<<<< HEAD
static DEFINE_CLOCK_DATA(cd);

static inline unsigned long long notrace _omap_mpu_sched_clock(void)
{
	u32 cyc = ~omap_mpu_timer_read(1);
	return cyc_to_sched_clock(&cd, cyc, (u32)~0);
}

#ifndef CONFIG_OMAP_32K_TIMER
unsigned long long notrace sched_clock(void)
{
	return _omap_mpu_sched_clock();
}
#else
static unsigned long long notrace omap_mpu_sched_clock(void)
{
	return _omap_mpu_sched_clock();
}
#endif

static void notrace mpu_update_sched_clock(void)
{
	u32 cyc = ~omap_mpu_timer_read(1);
	update_sched_clock(&cd, cyc, (u32)~0);
=======
static u32 notrace omap_mpu_read_sched_clock(void)
{
	return ~omap_mpu_timer_read(1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static u32 notrace omap_mpu_read_sched_clock(void)
{
	return ~omap_mpu_timer_read(1);
>>>>>>> refs/remotes/origin/master
}

static void __init omap_init_clocksource(unsigned long rate)
{
	omap_mpu_timer_regs_t __iomem *timer = omap_mpu_timer_base(1);
	static char err[] __initdata = KERN_ERR
			"%s: can't register clocksource!\n";

	omap_mpu_timer_start(1, ~0, 1);
<<<<<<< HEAD
<<<<<<< HEAD
	init_sched_clock(&cd, mpu_update_sched_clock, 32, rate);
=======
	setup_sched_clock(omap_mpu_read_sched_clock, 32, rate);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	setup_sched_clock(omap_mpu_read_sched_clock, 32, rate);
>>>>>>> refs/remotes/origin/master

	if (clocksource_mmio_init(&timer->read_tim, "mpu_timer2", rate,
			300, 32, clocksource_mmio_readl_down))
		printk(err, "mpu_timer2");
}

static void __init omap_mpu_timer_init(void)
{
	struct clk	*ck_ref = clk_get(NULL, "ck_ref");
	unsigned long	rate;

	BUG_ON(IS_ERR(ck_ref));

	rate = clk_get_rate(ck_ref);
	clk_put(ck_ref);

	/* PTV = 0 */
	rate /= 2;

	omap_init_mpu_timer(rate);
	omap_init_clocksource(rate);
}

#else
static inline void omap_mpu_timer_init(void)
{
	pr_err("Bogus timer, should not happen\n");
}
#endif	/* CONFIG_OMAP_MPU_TIMER */

<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_OMAP_MPU_TIMER) && defined(CONFIG_OMAP_32K_TIMER)
static unsigned long long (*preferred_sched_clock)(void);

unsigned long long notrace sched_clock(void)
{
	if (!preferred_sched_clock)
		return 0;

	return preferred_sched_clock();
}

static inline void preferred_sched_clock_init(bool use_32k_sched_clock)
{
	if (use_32k_sched_clock)
		preferred_sched_clock = omap_32k_sched_clock;
	else
		preferred_sched_clock = omap_mpu_sched_clock;
}
#else
static inline void preferred_sched_clock_init(bool use_32k_sched_clcok)
{
}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
static inline int omap_32k_timer_usable(void)
{
	int res = false;

	if (cpu_is_omap730() || cpu_is_omap15xx())
		return res;

#ifdef CONFIG_OMAP_32K_TIMER
	res = omap_32k_timer_init();
#endif

	return res;
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * ---------------------------------------------------------------------------
 * Timer initialization
 * ---------------------------------------------------------------------------
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void __init omap_timer_init(void)
{
	if (omap_32k_timer_usable()) {
		preferred_sched_clock_init(1);
	} else {
		omap_mpu_timer_init();
		preferred_sched_clock_init(0);
	}
}

struct sys_timer omap_timer = {
	.init		= omap_timer_init,
=======
static void __init omap1_timer_init(void)
{
	if (!omap_32k_timer_usable())
		omap_mpu_timer_init();
}

struct sys_timer omap1_timer = {
	.init		= omap1_timer_init,
>>>>>>> refs/remotes/origin/cm-10.0
};
=======
void __init omap1_timer_init(void)
{
	if (omap_32k_timer_init() != 0)
		omap_mpu_timer_init();
}
>>>>>>> refs/remotes/origin/master
