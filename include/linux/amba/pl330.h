/* linux/include/linux/amba/pl330.h
 *
 * Copyright (C) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassi.brar@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef	__AMBA_PL330_H_
#define	__AMBA_PL330_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/hardware/pl330.h>

struct dma_pl330_peri {
	/*
	 * Peri_Req i/f of the DMAC that is
	 * peripheral could be reached from.
	 */
	u8 peri_id; /* {0, 31} */
	enum pl330_reqtype rqtype;

	/* For M->D and D->M Channels */
	int burst_sz; /* in power of 2 */
	dma_addr_t fifo_addr;
};
=======
#include <linux/dmaengine.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/dmaengine.h>
>>>>>>> refs/remotes/origin/master

struct dma_pl330_platdata {
	/*
	 * Number of valid peripherals connected to DMAC.
	 * This may be different from the value read from
	 * CR0, as the PL330 implementation might have 'holes'
	 * in the peri list or the peri could also be reached
	 * from another DMAC which the platform prefers.
	 */
	u8 nr_valid_peri;
	/* Array of valid peripherals */
<<<<<<< HEAD
<<<<<<< HEAD
	struct dma_pl330_peri *peri;
=======
	u8 *peri_id;
	/* Operational capabilities */
	dma_cap_mask_t cap_mask;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 *peri_id;
	/* Operational capabilities */
	dma_cap_mask_t cap_mask;
>>>>>>> refs/remotes/origin/master
	/* Bytes to allocate for MC buffer */
	unsigned mcbuf_sz;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
extern bool pl330_filter(struct dma_chan *chan, void *param);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern bool pl330_filter(struct dma_chan *chan, void *param);
>>>>>>> refs/remotes/origin/master
#endif	/* __AMBA_PL330_H_ */
