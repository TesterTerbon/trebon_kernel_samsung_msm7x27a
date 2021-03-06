/*
 * Copyright 2008-2011 DENX Software Engineering GmbH
 * Author: Heiko Schocher <hs@denx.de>
 *
 * Description:
<<<<<<< HEAD
 * Keymile KMETER1 board specific routines.
=======
 * Keymile 83xx platform specific routines.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/reboot.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/major.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/root_dev.h>
#include <linux/initrd.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/time.h>
#include <asm/io.h>
=======
#include <linux/atomic.h>
#include <linux/time.h>
#include <linux/io.h>
>>>>>>> refs/remotes/origin/master
#include <asm/machdep.h>
#include <asm/ipic.h>
#include <asm/irq.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include <asm/qe.h>
#include <asm/qe_ic.h>

#include "mpc83xx.h"

#define SVR_REV(svr)    (((svr) >>  0) & 0xFFFF) /* Revision field */
<<<<<<< HEAD
=======

static void quirk_mpc8360e_qe_enet10(void)
{
	/*
	 * handle mpc8360E Erratum QE_ENET10:
	 * RGMII AC values do not meet the specification
	 */
	uint svid = mfspr(SPRN_SVR);
	struct	device_node *np_par;
	struct	resource res;
	void	__iomem *base;
	int	ret;

	np_par = of_find_node_by_name(NULL, "par_io");
	if (np_par == NULL) {
		pr_warn("%s couldn;t find par_io node\n", __func__);
		return;
	}
	/* Map Parallel I/O ports registers */
	ret = of_address_to_resource(np_par, 0, &res);
	if (ret) {
		pr_warn("%s couldn;t map par_io registers\n", __func__);
		return;
	}

	base = ioremap(res.start, res.end - res.start + 1);

	/*
	 * set output delay adjustments to default values according
	 * table 5 in Errata Rev. 5, 9/2011:
	 *
	 * write 0b01 to UCC1 bits 18:19
	 * write 0b01 to UCC2 option 1 bits 4:5
	 * write 0b01 to UCC2 option 2 bits 16:17
	 */
	clrsetbits_be32((base + 0xa8), 0x0c00f000, 0x04005000);

	/*
	 * set output delay adjustments to default values according
	 * table 3-13 in Reference Manual Rev.3 05/2010:
	 *
	 * write 0b01 to UCC2 option 2 bits 16:17
	 * write 0b0101 to UCC1 bits 20:23
	 * write 0b0101 to UCC2 option 1 bits 24:27
	 */
	clrsetbits_be32((base + 0xac), 0x0000cff0, 0x00004550);

	if (SVR_REV(svid) == 0x0021) {
		/*
		 * UCC2 option 1: write 0b1010 to bits 24:27
		 * at address IMMRBAR+0x14AC
		 */
		clrsetbits_be32((base + 0xac), 0x000000f0, 0x000000a0);
	} else if (SVR_REV(svid) == 0x0020) {
		/*
		 * UCC1: write 0b11 to bits 18:19
		 * at address IMMRBAR+0x14A8
		 */
		setbits32((base + 0xa8), 0x00003000);

		/*
		 * UCC2 option 1: write 0b11 to bits 4:5
		 * at address IMMRBAR+0x14A8
		 */
		setbits32((base + 0xa8), 0x0c000000);

		/*
		 * UCC2 option 2: write 0b11 to bits 16:17
		 * at address IMMRBAR+0x14AC
		 */
		setbits32((base + 0xac), 0x0000c000);
	}
	iounmap(base);
	of_node_put(np_par);
}

>>>>>>> refs/remotes/origin/master
/* ************************************************************************
 *
 * Setup the architecture
 *
 */
static void __init mpc83xx_km_setup_arch(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct device_node *np;
=======
#ifdef CONFIG_QUICC_ENGINE
	struct device_node *np;
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_QUICC_ENGINE
	struct device_node *np;
#endif
>>>>>>> refs/remotes/origin/master

	if (ppc_md.progress)
		ppc_md.progress("kmpbec83xx_setup_arch()", 0);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PCI
	for_each_compatible_node(np, "pci", "fsl,mpc8349-pci")
		mpc83xx_add_bridge(np);
#endif
=======
	mpc83xx_setup_pci();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mpc83xx_setup_pci();
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_QUICC_ENGINE
	qe_reset();

	np = of_find_node_by_name(NULL, "par_io");
	if (np != NULL) {
		par_io_init(np);
		of_node_put(np);

		for_each_node_by_name(np, "spi")
			par_io_of_config(np);

<<<<<<< HEAD
		for (np = NULL; (np = of_find_node_by_name(np, "ucc")) != NULL;)
			par_io_of_config(np);
	}

	np = of_find_compatible_node(NULL, "network", "ucc_geth");
	if (np != NULL) {
		uint svid;

		/* handle mpc8360ea rev.2.1 erratum 2: RGMII Timing */
		svid = mfspr(SPRN_SVR);
		if (SVR_REV(svid) == 0x0021) {
			struct	device_node *np_par;
			struct	resource res;
			void	__iomem *base;
			int	ret;

			np_par = of_find_node_by_name(NULL, "par_io");
			if (np_par == NULL) {
				printk(KERN_WARNING "%s couldn;t find par_io node\n",
					__func__);
				return;
			}
			/* Map Parallel I/O ports registers */
			ret = of_address_to_resource(np_par, 0, &res);
			if (ret) {
				printk(KERN_WARNING "%s couldn;t map par_io registers\n",
					__func__);
				return;
			}
<<<<<<< HEAD
			base = ioremap(res.start, res.end - res.start + 1);
=======
			base = ioremap(res.start, resource_size(&res));
>>>>>>> refs/remotes/origin/cm-10.0

			/*
			 * IMMR + 0x14A8[4:5] = 11 (clk delay for UCC 2)
			 * IMMR + 0x14A8[18:19] = 11 (clk delay for UCC 1)
			 */
			setbits32((base + 0xa8), 0x0c003000);

			/*
			 * IMMR + 0x14AC[20:27] = 10101010
			 * (data delay for both UCC's)
			 */
			clrsetbits_be32((base + 0xac), 0xff0, 0xaa0);
			iounmap(base);
			of_node_put(np_par);
		}
		of_node_put(np);
	}
#endif				/* CONFIG_QUICC_ENGINE */
}

<<<<<<< HEAD
static struct of_device_id kmpbec83xx_ids[] = {
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .compatible = "simple-bus", },
	{ .type = "qe", },
	{ .compatible = "fsl,qe", },
	{},
};

static int __init kmeter_declare_of_platform_devices(void)
{
	/* Publish the QE devices */
	of_platform_bus_probe(NULL, kmpbec83xx_ids, NULL);

	return 0;
}
machine_device_initcall(mpc83xx_km, kmeter_declare_of_platform_devices);

static void __init mpc83xx_km_init_IRQ(void)
{
	struct device_node *np;

	np = of_find_compatible_node(NULL, NULL, "fsl,pq2pro-pic");
	if (!np) {
		np = of_find_node_by_type(NULL, "ipic");
		if (!np)
			return;
	}

	ipic_init(np, 0);

	/* Initialize the default interrupt mapping priorities,
	 * in case the boot rom changed something on us.
	 */
	ipic_set_default_priority();
	of_node_put(np);

#ifdef CONFIG_QUICC_ENGINE
	np = of_find_compatible_node(NULL, NULL, "fsl,qe-ic");
	if (!np) {
		np = of_find_node_by_type(NULL, "qeic");
		if (!np)
			return;
	}
	qe_ic_init(np, 0, qe_ic_cascade_low_ipic, qe_ic_cascade_high_ipic);
	of_node_put(np);
#endif				/* CONFIG_QUICC_ENGINE */
}
=======
machine_device_initcall(mpc83xx_km, mpc83xx_declare_of_platform_devices);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		for_each_node_by_name(np, "ucc")
			par_io_of_config(np);

		/* Only apply this quirk when par_io is available */
		np = of_find_compatible_node(NULL, "network", "ucc_geth");
		if (np != NULL) {
			quirk_mpc8360e_qe_enet10();
			of_node_put(np);
		}
	}
#endif	/* CONFIG_QUICC_ENGINE */
}

machine_device_initcall(mpc83xx_km, mpc83xx_declare_of_platform_devices);
>>>>>>> refs/remotes/origin/master

/* list of the supported boards */
static char *board[] __initdata = {
	"Keymile,KMETER1",
	"Keymile,kmpbec8321",
	NULL
};

/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init mpc83xx_km_probe(void)
{
	unsigned long node = of_get_flat_dt_root();
	int i = 0;

	while (board[i]) {
		if (of_flat_dt_is_compatible(node, board[i]))
			break;
		i++;
	}
	return (board[i] != NULL);
}

define_machine(mpc83xx_km) {
	.name		= "mpc83xx-km-platform",
	.probe		= mpc83xx_km_probe,
	.setup_arch	= mpc83xx_km_setup_arch,
<<<<<<< HEAD
<<<<<<< HEAD
	.init_IRQ	= mpc83xx_km_init_IRQ,
=======
	.init_IRQ	= mpc83xx_ipic_and_qe_init_IRQ,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_IRQ	= mpc83xx_ipic_and_qe_init_IRQ,
>>>>>>> refs/remotes/origin/master
	.get_irq	= ipic_get_irq,
	.restart	= mpc83xx_restart,
	.time_init	= mpc83xx_time_init,
	.calibrate_decr	= generic_calibrate_decr,
	.progress	= udbg_progress,
};
