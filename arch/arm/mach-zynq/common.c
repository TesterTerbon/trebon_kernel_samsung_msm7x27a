/*
 * This file contains common code that is intended to be used across
 * boards so that it's not replicated.
 *
 *  Copyright (C) 2011 Xilinx
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
<<<<<<< HEAD
=======
#include <linux/clk/zynq.h>
#include <linux/clocksource.h>
#include <linux/of_address.h>
>>>>>>> refs/remotes/origin/master
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
<<<<<<< HEAD
#include <asm/mach-types.h>
#include <asm/page.h>
#include <asm/hardware/gic.h>
#include <asm/hardware/cache-l2x0.h>

#include <mach/zynq_soc.h>
#include <mach/clkdev.h>
#include "common.h"

=======
#include <asm/mach/time.h>
#include <asm/mach-types.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/smp_scu.h>
#include <asm/hardware/cache-l2x0.h>

#include "common.h"

void __iomem *zynq_scu_base;

>>>>>>> refs/remotes/origin/master
static struct of_device_id zynq_of_bus_ids[] __initdata = {
	{ .compatible = "simple-bus", },
	{}
};

<<<<<<< HEAD
/**
 * xilinx_init_machine() - System specific initialization, intended to be
 *			   called from board specific initialization.
 */
static void __init xilinx_init_machine(void)
{
#ifdef CONFIG_CACHE_L2X0
	/*
	 * 64KB way size, 8-way associativity, parity disabled
	 */
	l2x0_init(PL310_L2CC_BASE, 0x02060000, 0xF0F0FFFF);
#endif

	of_platform_bus_probe(NULL, zynq_of_bus_ids, NULL);
}

/**
 * xilinx_irq_init() - Interrupt controller initialization for the GIC.
 */
static void __init xilinx_irq_init(void)
{
	gic_init(0, 29, SCU_GIC_DIST_BASE, SCU_GIC_CPU_BASE);
}

/* The minimum devices needed to be mapped before the VM system is up and
 * running include the GIC, UART and Timer Counter.
 */

static struct map_desc io_desc[] __initdata = {
	{
		.virtual	= TTC0_VIRT,
		.pfn		= __phys_to_pfn(TTC0_PHYS),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= SCU_PERIPH_VIRT,
		.pfn		= __phys_to_pfn(SCU_PERIPH_PHYS),
		.length		= SZ_8K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= PL310_L2CC_VIRT,
		.pfn		= __phys_to_pfn(PL310_L2CC_PHYS),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},

#ifdef CONFIG_DEBUG_LL
	{
		.virtual	= UART0_VIRT,
		.pfn		= __phys_to_pfn(UART0_PHYS),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
#endif

};

/**
 * xilinx_map_io() - Create memory mappings needed for early I/O.
 */
static void __init xilinx_map_io(void)
{
	iotable_init(io_desc, ARRAY_SIZE(io_desc));
}

static const char *xilinx_dt_match[] = {
	"xlnx,zynq-ep107",
	NULL
};

MACHINE_START(XILINX_EP107, "Xilinx Zynq Platform")
	.map_io		= xilinx_map_io,
	.init_irq	= xilinx_irq_init,
	.handle_irq	= gic_handle_irq,
	.init_machine	= xilinx_init_machine,
	.timer		= &xttcpss_sys_timer,
	.dt_compat	= xilinx_dt_match,
=======
static struct platform_device zynq_cpuidle_device = {
	.name = "cpuidle-zynq",
};

/**
 * zynq_init_machine - System specific initialization, intended to be
 *		       called from board specific initialization.
 */
static void __init zynq_init_machine(void)
{
	/*
	 * 64KB way size, 8-way associativity, parity disabled
	 */
	l2x0_of_init(0x02060000, 0xF0F0FFFF);

	of_platform_bus_probe(NULL, zynq_of_bus_ids, NULL);

	platform_device_register(&zynq_cpuidle_device);
}

static void __init zynq_timer_init(void)
{
	zynq_slcr_init();
	clocksource_of_init();
}

static struct map_desc zynq_cortex_a9_scu_map __initdata = {
	.length	= SZ_256,
	.type	= MT_DEVICE,
};

static void __init zynq_scu_map_io(void)
{
	unsigned long base;

	base = scu_a9_get_base();
	zynq_cortex_a9_scu_map.pfn = __phys_to_pfn(base);
	/* Expected address is in vmalloc area that's why simple assign here */
	zynq_cortex_a9_scu_map.virtual = base;
	iotable_init(&zynq_cortex_a9_scu_map, 1);
	zynq_scu_base = (void __iomem *)base;
	BUG_ON(!zynq_scu_base);
}

/**
 * zynq_map_io - Create memory mappings needed for early I/O.
 */
static void __init zynq_map_io(void)
{
	debug_ll_io_init();
	zynq_scu_map_io();
}

static void zynq_system_reset(enum reboot_mode mode, const char *cmd)
{
	zynq_slcr_system_reset();
}

static const char * const zynq_dt_match[] = {
	"xlnx,zynq-7000",
	NULL
};

DT_MACHINE_START(XILINX_EP107, "Xilinx Zynq Platform")
	.smp		= smp_ops(zynq_smp_ops),
	.map_io		= zynq_map_io,
	.init_machine	= zynq_init_machine,
	.init_time	= zynq_timer_init,
	.dt_compat	= zynq_dt_match,
	.restart	= zynq_system_reset,
>>>>>>> refs/remotes/origin/master
MACHINE_END
