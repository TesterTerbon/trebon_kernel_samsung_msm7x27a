/*
 * Support for AM3517/05 Craneboard
 * http://www.mistralsolutions.com/products/craneboard.php
 *
 * Copyright (C) 2010 Mistral Solutions Pvt Ltd. <www.mistralsolutions.com>
 * Author: R.Srinath <srinath@mistralsolutions.com>
 *
 * Based on mach-omap2/board-am3517evm.c
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as  published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any kind,
 * whether express or implied; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
<<<<<<< HEAD

#include <mach/hardware.h>
=======
#include <linux/mfd/tps65910.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

>>>>>>> refs/remotes/origin/master
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

<<<<<<< HEAD
#include <plat/board.h>
<<<<<<< HEAD
#include <plat/common.h>
=======
#include "common.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include <plat/usb.h>

=======
#include "common.h"
#include "common-board-devices.h"
#include "board-flash.h"

#include "am35xx-emac.h"
>>>>>>> refs/remotes/origin/master
#include "mux.h"
#include "control.h"

#define GPIO_USB_POWER		35
#define GPIO_USB_NRESET		38

<<<<<<< HEAD

/* Board initialization */
static struct omap_board_config_kernel am3517_crane_config[] __initdata = {
};

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
<<<<<<< HEAD
#else
#define board_mux	NULL
#endif

static void __init am3517_crane_init_early(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
}

=======
#endif

>>>>>>> refs/remotes/origin/cm-10.0
static struct usbhs_omap_board_data usbhs_bdata __initdata = {
	.port_mode[0] = OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[1] = OMAP_USBHS_PORT_MODE_UNUSED,
	.port_mode[2] = OMAP_USBHS_PORT_MODE_UNUSED,

	.phy_reset  = true,
	.reset_gpio_port[0]  = GPIO_USB_NRESET,
	.reset_gpio_port[1]  = -EINVAL,
	.reset_gpio_port[2]  = -EINVAL
};

static void __init am3517_crane_init(void)
{
	int ret;

	omap3_mux_init(board_mux, OMAP_PACKAGE_CBB);
	omap_serial_init();
<<<<<<< HEAD
=======
	omap_sdrc_init(NULL, NULL);
>>>>>>> refs/remotes/origin/cm-10.0

	omap_board_config = am3517_crane_config;
	omap_board_config_size = ARRAY_SIZE(am3517_crane_config);
=======
#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	OMAP3_MUX(SYS_NIRQ, OMAP_MUX_MODE0 | OMAP_PIN_INPUT_PULLUP),
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#endif

static struct usbhs_phy_data phy_data[] __initdata = {
	{
		.port = 1,
		.reset_gpio = GPIO_USB_NRESET,
		.vcc_gpio = GPIO_USB_POWER,
		.vcc_polarity = 1,
	},
};

static struct usbhs_omap_platform_data usbhs_bdata __initdata = {
	.port_mode[0] = OMAP_EHCI_PORT_MODE_PHY,
};

static struct mtd_partition crane_nand_partitions[] = {
	{
		.name		= "X-Loader",
		.offset		= 0,
		.size		= 4 * NAND_BLOCK_SIZE,
		.mask_flags	= MTD_WRITEABLE,
	},
	{
		.name		= "U-Boot",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 14 * NAND_BLOCK_SIZE,
		.mask_flags	= MTD_WRITEABLE,
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 2 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 40 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
	},
};

static struct tps65910_board tps65910_pdata = {
	.irq = 7 + OMAP_INTC_START,
	.en_ck32k_xtal = true,
};

static struct i2c_board_info __initdata tps65910_board_info[] = {
	{
		I2C_BOARD_INFO("tps65910", 0x2d),
		.platform_data = &tps65910_pdata,
	},
};

static void __init am3517_crane_i2c_init(void)
{
	omap_register_i2c_bus(1, 2600, tps65910_board_info,
			ARRAY_SIZE(tps65910_board_info));
}

static void __init am3517_crane_init(void)
{
	omap3_mux_init(board_mux, OMAP_PACKAGE_CBB);
	omap_serial_init();
	omap_sdrc_init(NULL, NULL);
	board_nand_init(crane_nand_partitions,
			ARRAY_SIZE(crane_nand_partitions), 0,
			NAND_BUSWIDTH_16, NULL);
	am3517_crane_i2c_init();
>>>>>>> refs/remotes/origin/master

	/* Configure GPIO for EHCI port */
	if (omap_mux_init_gpio(GPIO_USB_NRESET, OMAP_PIN_OUTPUT)) {
		pr_err("Can not configure mux for GPIO_USB_NRESET %d\n",
			GPIO_USB_NRESET);
		return;
	}

	if (omap_mux_init_gpio(GPIO_USB_POWER, OMAP_PIN_OUTPUT)) {
		pr_err("Can not configure mux for GPIO_USB_POWER %d\n",
			GPIO_USB_POWER);
		return;
	}

<<<<<<< HEAD
	ret = gpio_request_one(GPIO_USB_POWER, GPIOF_OUT_INIT_HIGH,
			       "usb_ehci_enable");
	if (ret < 0) {
		pr_err("Can not request GPIO %d\n", GPIO_USB_POWER);
		return;
	}

	usbhs_init(&usbhs_bdata);
}

MACHINE_START(CRANEBOARD, "AM3517/05 CRANEBOARD")
<<<<<<< HEAD
	.boot_params	= 0x80000100,
	.reserve	= omap_reserve,
	.map_io		= omap3_map_io,
	.init_early	= am3517_crane_init_early,
	.init_irq	= omap_init_irq,
	.init_machine	= am3517_crane_init,
	.timer		= &omap_timer,
=======
=======
	usbhs_init_phys(phy_data, ARRAY_SIZE(phy_data));
	usbhs_init(&usbhs_bdata);
	am35xx_emac_init(AM35XX_DEFAULT_MDIO_FREQUENCY, 1);
}

MACHINE_START(CRANEBOARD, "AM3517/05 CRANEBOARD")
>>>>>>> refs/remotes/origin/master
	.atag_offset	= 0x100,
	.reserve	= omap_reserve,
	.map_io		= omap3_map_io,
	.init_early	= am35xx_init_early,
	.init_irq	= omap3_init_irq,
	.handle_irq	= omap3_intc_handle_irq,
	.init_machine	= am3517_crane_init,
<<<<<<< HEAD
	.timer		= &omap3_timer,
	.restart	= omap_prcm_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_late	= am35xx_init_late,
	.init_time	= omap3_sync32k_timer_init,
	.restart	= omap3xxx_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END
