/*
 * arch/arm/mach-at91/board-eb01.c
 *
 * (C) Copyright 2007, Greg Ungerer <gerg@snapgear.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
<<<<<<< HEAD
#include <mach/board.h>
=======

#include "at91_aic.h"
#include "board.h"
>>>>>>> refs/remotes/origin/master
#include "generic.h"

static void __init at91eb01_init_irq(void)
{
	at91x40_init_interrupts(NULL);
}

static void __init at91eb01_init_early(void)
{
	at91x40_initialize(40000000);
}

MACHINE_START(AT91EB01, "Atmel AT91 EB01")
	/* Maintainer: Greg Ungerer <gerg@snapgear.com> */
<<<<<<< HEAD
	.timer		= &at91x40_timer,
=======
	.init_time	= at91x40_timer_init,
	.handle_irq	= at91_aic_handle_irq,
>>>>>>> refs/remotes/origin/master
	.init_early	= at91eb01_init_early,
	.init_irq	= at91eb01_init_irq,
MACHINE_END

