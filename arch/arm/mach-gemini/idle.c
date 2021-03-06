/*
 * arch/arm/mach-gemini/idle.c
 */

#include <linux/init.h>
#include <asm/system.h>
#include <asm/proc-fns.h>

static void gemini_idle(void)
{
	/*
	 * Because of broken hardware we have to enable interrupts or the CPU
	 * will never wakeup... Acctualy it is not very good to enable
	 * interrupts first since scheduler can miss a tick, but there is
	 * no other way around this. Platforms that needs it for power saving
<<<<<<< HEAD
	 * should call enable_hlt() in init code, since by default it is
	 * disabled.
	 */
=======
	 * should enable it in init code, since by default it is
	 * disabled.
	 */

	/* FIXME: Enabling interrupts here is racy! */
>>>>>>> refs/remotes/origin/master
	local_irq_enable();
	cpu_do_idle();
}

static int __init gemini_idle_init(void)
{
	arm_pm_idle = gemini_idle;
	return 0;
}

arch_initcall(gemini_idle_init);
