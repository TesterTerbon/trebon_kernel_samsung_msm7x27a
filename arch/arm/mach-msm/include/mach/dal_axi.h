/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
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
#ifndef _DAL_AXI_H
#define _DAL_AXI_H

#include <mach/dal.h>

int set_grp2d_async(void);
int set_grp3d_async(void);
int set_grp_xbar_async(void);
<<<<<<< HEAD
<<<<<<< HEAD

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
int axi_allocate(int mode);
int axi_free(int mode);
int axi_halt(int port);
#define AXI_FLOW_VIEWFINDER_HI	243
#define AXI_HALT_PORT_VFE	7
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#endif  /* _DAL_AXI_H */
