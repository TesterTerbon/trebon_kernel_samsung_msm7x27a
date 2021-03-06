/*
 * linux/arch/arm/mach-omap2/board-omap3touchbook.c
 *
 * Copyright (C) 2009 Always Innovating
 *
 * Modified from mach-omap2/board-omap3beagleboard.c
 *
 * Initial code: Grégoire Gentil, Tim Yamin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <linux/mmc/host.h>
<<<<<<< HEAD

#include <plat/mcspi.h>
=======
#include <linux/usb/phy.h>

#include <linux/platform_data/spi-omap2-mcspi.h>
>>>>>>> refs/remotes/origin/master
#include <linux/spi/spi.h>

#include <linux/spi/ads7846.h>

#include <linux/regulator/machine.h>
#include <linux/i2c/twl.h>

<<<<<<< HEAD
#include <mach/hardware.h>
=======
>>>>>>> refs/remotes/origin/master
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <plat/board.h>
#include <plat/common.h>
=======
#include <asm/system_info.h>

#include <plat/board.h>
#include "common.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include <plat/gpmc.h>
#include <plat/nand.h>
#include <plat/usb.h>

#include "mux.h"
#include "hsmmc.h"
<<<<<<< HEAD
#include "timer-gp.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/system_info.h>

#include "common.h"
#include "gpmc.h"
#include <linux/platform_data/mtd-nand-omap2.h>

#include "mux.h"
#include "hsmmc.h"
#include "board-flash.h"
>>>>>>> refs/remotes/origin/master
#include "common-board-devices.h"

#include <asm/setup.h>

#define OMAP3_AC_GPIO		136
#define OMAP3_TS_GPIO		162
#define TB_BL_PWM_TIMER		9
#define TB_KILL_POWER_GPIO	168

<<<<<<< HEAD
=======
#define	NAND_CS			0

>>>>>>> refs/remotes/origin/master
static unsigned long touchbook_revision;

static struct mtd_partition omap3touchbook_nand_partitions[] = {
	/* All the partition sizes are listed in terms of NAND block size */
	{
		.name		= "X-Loader",
		.offset		= 0,
		.size		= 4 * NAND_BLOCK_SIZE,
		.mask_flags	= MTD_WRITEABLE,	/* force read-only */
	},
	{
		.name		= "U-Boot",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x80000 */
		.size		= 15 * NAND_BLOCK_SIZE,
		.mask_flags	= MTD_WRITEABLE,	/* force read-only */
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x260000 */
		.size		= 1 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x280000 */
		.size		= 32 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x680000 */
		.size		= MTDPART_SIZ_FULL,
	},
};

#include "sdram-micron-mt46h32m32lf-6.h"

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA,
		.gpio_wp	= 29,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.deferred	= true,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.deferred	= true,
>>>>>>> refs/remotes/origin/master
	},
	{}	/* Terminator */
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct platform_device omap3_touchbook_lcd_device = {
	.name		= "omap3touchbook_lcd",
	.id		= -1,
};

static struct omap_lcd_config omap3_touchbook_lcd_config __initdata = {
	.ctrl_name	= "internal",
};

static struct regulator_consumer_supply touchbook_vmmc1_supply = {
	.supply			= "vmmc",
};

static struct regulator_consumer_supply touchbook_vsim_supply = {
	.supply			= "vmmc_aux",
=======
=======
>>>>>>> refs/remotes/origin/master
static struct regulator_consumer_supply touchbook_vmmc1_supply[] = {
	REGULATOR_SUPPLY("vmmc", "omap_hsmmc.0"),
};

static struct regulator_consumer_supply touchbook_vsim_supply[] = {
	REGULATOR_SUPPLY("vmmc_aux", "omap_hsmmc.0"),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static struct gpio_led gpio_leds[];

static int touchbook_twl_gpio_setup(struct device *dev,
		unsigned gpio, unsigned ngpio)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (system_rev >= 0x20 && system_rev <= 0x34301000) {
		omap_mux_init_gpio(23, OMAP_PIN_INPUT);
		mmc[0].gpio_wp = 23;
	} else {
		omap_mux_init_gpio(29, OMAP_PIN_INPUT);
	}
	/* gpio + 0 is "mmc0_cd" (input/IRQ) */
	mmc[0].gpio_cd = gpio + 0;
	omap2_hsmmc_init(mmc);

	/* link regulators to MMC adapters */
	touchbook_vmmc1_supply.dev = mmc[0].dev;
	touchbook_vsim_supply.dev = mmc[0].dev;
=======
	/* gpio + 0 is "mmc0_cd" (input/IRQ) */
	mmc[0].gpio_cd = gpio + 0;
	omap_hsmmc_late_init(mmc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* gpio + 0 is "mmc0_cd" (input/IRQ) */
	mmc[0].gpio_cd = gpio + 0;
	omap_hsmmc_late_init(mmc);
>>>>>>> refs/remotes/origin/master

	/* REVISIT: need ehci-omap hooks for external VBUS
	 * power switch and overcurrent detect
	 */
	gpio_request_one(gpio + 1, GPIOF_IN, "EHCI_nOC");

	/* TWL4030_GPIO_MAX + 0 == ledA, EHCI nEN_USB_PWR (out, active low) */
	gpio_request_one(gpio + TWL4030_GPIO_MAX, GPIOF_OUT_INIT_LOW,
			 "nEN_USB_PWR");

	/* TWL4030_GPIO_MAX + 1 == ledB, PMU_STAT (out, active low LED) */
	gpio_leds[2].gpio = gpio + TWL4030_GPIO_MAX + 1;

	return 0;
}

static struct twl4030_gpio_platform_data touchbook_gpio_data = {
<<<<<<< HEAD
	.gpio_base	= OMAP_MAX_GPIO_LINES,
	.irq_base	= TWL4030_GPIO_IRQ_BASE,
	.irq_end	= TWL4030_GPIO_IRQ_END,
=======
>>>>>>> refs/remotes/origin/master
	.use_leds	= true,
	.pullups	= BIT(1),
	.pulldowns	= BIT(2) | BIT(6) | BIT(7) | BIT(8) | BIT(13)
				| BIT(15) | BIT(16) | BIT(17),
	.setup		= touchbook_twl_gpio_setup,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct regulator_consumer_supply touchbook_vdac_supply = {
	.supply		= "vdac",
	.dev		= &omap3_touchbook_lcd_device.dev,
};

static struct regulator_consumer_supply touchbook_vdvi_supply = {
	.supply		= "vdvi",
	.dev		= &omap3_touchbook_lcd_device.dev,
=======
=======
>>>>>>> refs/remotes/origin/master
static struct regulator_consumer_supply touchbook_vdac_supply[] = {
{
	.supply		= "vdac",
},
};

static struct regulator_consumer_supply touchbook_vdvi_supply[] = {
{
	.supply		= "vdvi",
},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

/* VMMC1 for MMC1 pins CMD, CLK, DAT0..DAT3 (20 mA, plus card == max 220 mA) */
static struct regulator_init_data touchbook_vmmc1 = {
	.constraints = {
		.min_uV			= 1850000,
		.max_uV			= 3150000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE
					| REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
<<<<<<< HEAD
<<<<<<< HEAD
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &touchbook_vmmc1_supply,
=======
	.num_consumer_supplies	= ARRAY_SIZE(touchbook_vmmc1_supply),
	.consumer_supplies	= touchbook_vmmc1_supply,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.num_consumer_supplies	= ARRAY_SIZE(touchbook_vmmc1_supply),
	.consumer_supplies	= touchbook_vmmc1_supply,
>>>>>>> refs/remotes/origin/master
};

/* VSIM for MMC1 pins DAT4..DAT7 (2 mA, plus card == max 50 mA) */
static struct regulator_init_data touchbook_vsim = {
	.constraints = {
		.min_uV			= 1800000,
		.max_uV			= 3000000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE
					| REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
<<<<<<< HEAD
<<<<<<< HEAD
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &touchbook_vsim_supply,
=======
	.num_consumer_supplies	= ARRAY_SIZE(touchbook_vsim_supply),
	.consumer_supplies	= touchbook_vsim_supply,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.num_consumer_supplies	= ARRAY_SIZE(touchbook_vsim_supply),
	.consumer_supplies	= touchbook_vsim_supply,
>>>>>>> refs/remotes/origin/master
};

static struct twl4030_platform_data touchbook_twldata = {
	/* platform_data for children goes here */
	.gpio		= &touchbook_gpio_data,
	.vmmc1		= &touchbook_vmmc1,
	.vsim		= &touchbook_vsim,
};

static struct i2c_board_info __initdata touchBook_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("bq27200", 0x55),
	},
};

static int __init omap3_touchbook_i2c_init(void)
{
	/* Standard TouchBook bus */
	omap3_pmic_get_config(&touchbook_twldata,
			TWL_COMMON_PDATA_USB | TWL_COMMON_PDATA_AUDIO,
			TWL_COMMON_REGULATOR_VDAC | TWL_COMMON_REGULATOR_VPLL2);
<<<<<<< HEAD
<<<<<<< HEAD
	touchbook_twldata.vdac->num_consumer_supplies = 1;
	touchbook_twldata.vdac->consumer_supplies = &touchbook_vdac_supply;
	touchbook_twldata.vpll2->constraints.name = "VDVI";
	touchbook_twldata.vpll2->num_consumer_supplies = 1;
	touchbook_twldata.vpll2->consumer_supplies = &touchbook_vdvi_supply;
=======
=======
>>>>>>> refs/remotes/origin/master

	touchbook_twldata.vdac->num_consumer_supplies =
					ARRAY_SIZE(touchbook_vdac_supply);
	touchbook_twldata.vdac->consumer_supplies = touchbook_vdac_supply;

	touchbook_twldata.vpll2->constraints.name = "VDVI";
	touchbook_twldata.vpll2->num_consumer_supplies =
					ARRAY_SIZE(touchbook_vdvi_supply);
	touchbook_twldata.vpll2->consumer_supplies = touchbook_vdvi_supply;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	omap3_pmic_init("twl4030", &touchbook_twldata);
	/* Additional TouchBook bus */
	omap_register_i2c_bus(3, 100, touchBook_i2c_boardinfo,
			ARRAY_SIZE(touchBook_i2c_boardinfo));

	return 0;
}

static struct ads7846_platform_data ads7846_pdata = {
	.x_min			= 100,
	.y_min			= 265,
	.x_max			= 3950,
	.y_max			= 3750,
	.x_plate_ohms		= 40,
	.pressure_max		= 255,
	.debounce_max		= 10,
	.debounce_tol		= 5,
	.debounce_rep		= 1,
	.gpio_pendown		= OMAP3_TS_GPIO,
	.keep_vref_on		= 1,
};

static struct gpio_led gpio_leds[] = {
	{
		.name			= "touchbook::usr0",
		.default_trigger	= "heartbeat",
		.gpio			= 150,
	},
	{
		.name			= "touchbook::usr1",
		.default_trigger	= "mmc0",
		.gpio			= 149,
	},
	{
		.name			= "touchbook::pmu_stat",
		.gpio			= -EINVAL,	/* gets replaced */
		.active_low		= true,
	},
};

static struct gpio_led_platform_data gpio_led_info = {
	.leds		= gpio_leds,
	.num_leds	= ARRAY_SIZE(gpio_leds),
};

static struct platform_device leds_gpio = {
	.name	= "leds-gpio",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_led_info,
	},
};

static struct gpio_keys_button gpio_buttons[] = {
	{
		.code			= BTN_EXTRA,
		.gpio			= 7,
		.desc			= "user",
		.wakeup			= 1,
	},
	{
		.code			= KEY_POWER,
		.gpio			= 183,
		.desc			= "power",
		.wakeup			= 1,
	},
};

static struct gpio_keys_platform_data gpio_key_info = {
	.buttons	= gpio_buttons,
	.nbuttons	= ARRAY_SIZE(gpio_buttons),
};

static struct platform_device keys_gpio = {
	.name	= "gpio-keys",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_key_info,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct omap_board_config_kernel omap3_touchbook_config[] __initdata = {
	{ OMAP_TAG_LCD,		&omap3_touchbook_lcd_config },
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#endif

<<<<<<< HEAD
<<<<<<< HEAD
static void __init omap3_touchbook_init_early(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(mt46h32m32lf6_sdrc_params,
				  mt46h32m32lf6_sdrc_params);
}

static void __init omap3_touchbook_init_irq(void)
{
	omap_init_irq();
#ifdef CONFIG_OMAP_32K_TIMER
	omap2_gp_clockevent_set_gptimer(12);
#endif
}

static struct platform_device *omap3_touchbook_devices[] __initdata = {
	&omap3_touchbook_lcd_device,
=======
static struct platform_device *omap3_touchbook_devices[] __initdata = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usbhs_phy_data phy_data[] __initdata = {
	{
		.port = 2,
		.reset_gpio = 147,
		.vcc_gpio = -EINVAL,
	},
};

static struct platform_device *omap3_touchbook_devices[] __initdata = {
>>>>>>> refs/remotes/origin/master
	&leds_gpio,
	&keys_gpio,
};

<<<<<<< HEAD
static const struct usbhs_omap_board_data usbhs_bdata __initconst = {

	.port_mode[0] = OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[1] = OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[2] = OMAP_USBHS_PORT_MODE_UNUSED,

	.phy_reset  = true,
	.reset_gpio_port[0]  = -EINVAL,
	.reset_gpio_port[1]  = 147,
	.reset_gpio_port[2]  = -EINVAL
=======
static struct usbhs_omap_platform_data usbhs_bdata __initdata = {
	.port_mode[0] = OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[1] = OMAP_EHCI_PORT_MODE_PHY,
>>>>>>> refs/remotes/origin/master
};

static void omap3_touchbook_poweroff(void)
{
	int pwr_off = TB_KILL_POWER_GPIO;

	if (gpio_request_one(pwr_off, GPIOF_OUT_INIT_LOW, "DVI reset") < 0)
		printk(KERN_ERR "Unable to get kill power GPIO\n");
}

static int __init early_touchbook_revision(char *p)
{
	if (!p)
		return 0;

	return strict_strtoul(p, 10, &touchbook_revision);
}
early_param("tbr", early_touchbook_revision);

static void __init omap3_touchbook_init(void)
{
	omap3_mux_init(board_mux, OMAP_PACKAGE_CBB);
<<<<<<< HEAD
<<<<<<< HEAD
	omap_board_config = omap3_touchbook_config;
	omap_board_config_size = ARRAY_SIZE(omap3_touchbook_config);

	pm_power_off = omap3_touchbook_poweroff;

=======
=======
>>>>>>> refs/remotes/origin/master

	pm_power_off = omap3_touchbook_poweroff;

	if (system_rev >= 0x20 && system_rev <= 0x34301000) {
		omap_mux_init_gpio(23, OMAP_PIN_INPUT);
		mmc[0].gpio_wp = 23;
	} else {
		omap_mux_init_gpio(29, OMAP_PIN_INPUT);
	}
	omap_hsmmc_init(mmc);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	omap3_touchbook_i2c_init();
	platform_add_devices(omap3_touchbook_devices,
			ARRAY_SIZE(omap3_touchbook_devices));
	omap_serial_init();
<<<<<<< HEAD
<<<<<<< HEAD
=======
	omap_sdrc_init(mt46h32m32lf6_sdrc_params,
				  mt46h32m32lf6_sdrc_params);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	omap_sdrc_init(mt46h32m32lf6_sdrc_params,
				  mt46h32m32lf6_sdrc_params);
>>>>>>> refs/remotes/origin/master

	omap_mux_init_gpio(170, OMAP_PIN_INPUT);
	/* REVISIT leave DVI powered down until it's needed ... */
	gpio_request_one(176, GPIOF_OUT_INIT_HIGH, "DVI_nPD");

	/* Touchscreen and accelerometer */
	omap_ads7846_init(4, OMAP3_TS_GPIO, 310, &ads7846_pdata);
<<<<<<< HEAD
	usb_musb_init(NULL);
	usbhs_init(&usbhs_bdata);
	omap_nand_flash_init(NAND_BUSWIDTH_16, omap3touchbook_nand_partitions,
			     ARRAY_SIZE(omap3touchbook_nand_partitions));
=======
	usb_bind_phy("musb-hdrc.0.auto", 0, "twl4030_usb");
	usb_musb_init(NULL);

	usbhs_init_phys(phy_data, ARRAY_SIZE(phy_data));
	usbhs_init(&usbhs_bdata);
	board_nand_init(omap3touchbook_nand_partitions,
			ARRAY_SIZE(omap3touchbook_nand_partitions), NAND_CS,
			NAND_BUSWIDTH_16, NULL);
>>>>>>> refs/remotes/origin/master

	/* Ensure SDRC pins are mux'd for self-refresh */
	omap_mux_init_signal("sdrc_cke0", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("sdrc_cke1", OMAP_PIN_OUTPUT);
}

MACHINE_START(TOUCHBOOK, "OMAP3 touchbook Board")
	/* Maintainer: Gregoire Gentil - http://www.alwaysinnovating.com */
<<<<<<< HEAD
<<<<<<< HEAD
	.boot_params	= 0x80000100,
	.reserve	= omap_reserve,
	.map_io		= omap3_map_io,
	.init_early	= omap3_touchbook_init_early,
	.init_irq	= omap3_touchbook_init_irq,
	.init_machine	= omap3_touchbook_init,
	.timer		= &omap_timer,
=======
=======
>>>>>>> refs/remotes/origin/master
	.atag_offset	= 0x100,
	.reserve	= omap_reserve,
	.map_io		= omap3_map_io,
	.init_early	= omap3430_init_early,
	.init_irq	= omap3_init_irq,
	.handle_irq	= omap3_intc_handle_irq,
	.init_machine	= omap3_touchbook_init,
<<<<<<< HEAD
	.timer		= &omap3_secure_timer,
	.restart	= omap_prcm_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_late	= omap3430_init_late,
	.init_time	= omap3_secure_sync32k_timer_init,
	.restart	= omap3xxx_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END
