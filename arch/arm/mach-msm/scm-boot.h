<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010, 2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2010, 2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __MACH_SCM_BOOT_H
#define __MACH_SCM_BOOT_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define SCM_BOOT_ADDR				0x1
#define SCM_FLAG_COLDBOOT_CPU1		0x01
#define SCM_FLAG_COLDBOOT_CPU2		0x08
#define SCM_FLAG_COLDBOOT_CPU3		0x20
#define SCM_FLAG_WARMBOOT_CPU1		0x02
#define SCM_FLAG_WARMBOOT_CPU0		0x04
#define SCM_FLAG_WARMBOOT_CPU2		0x10
#define SCM_FLAG_WARMBOOT_CPU3		0x40
<<<<<<< HEAD

<<<<<<< HEAD
int scm_set_boot_addr(void *addr, int flags);
=======
#ifdef CONFIG_MSM_SCM
int scm_set_boot_addr(void *addr, int flags);
#else
static inline int scm_set_boot_addr(void *addr, int flags) { return 0; }
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SCM_BOOT_ADDR			0x1
#define SCM_FLAG_COLDBOOT_CPU1		0x1
#define SCM_FLAG_WARMBOOT_CPU1		0x2
#define SCM_FLAG_WARMBOOT_CPU0		0x4

int scm_set_boot_addr(phys_addr_t addr, int flags);
>>>>>>> refs/remotes/origin/master
=======

#ifdef CONFIG_MSM_SCM
int scm_set_boot_addr(void *addr, int flags);
#else
static inline int scm_set_boot_addr(void *addr, int flags) { return 0; }
#endif
>>>>>>> refs/remotes/origin/cm-11.0

#endif
