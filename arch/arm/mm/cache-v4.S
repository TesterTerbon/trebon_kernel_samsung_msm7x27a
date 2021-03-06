/*
 *  linux/arch/arm/mm/cache-v4.S
 *
 *  Copyright (C) 1997-2002 Russell king
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/page.h>
#include "proc-macros.S"

/*
 *	flush_icache_all()
 *
 *	Unconditionally clean and invalidate the entire icache.
 */
ENTRY(v4_flush_icache_all)
	mov	pc, lr
ENDPROC(v4_flush_icache_all)

/*
 *	flush_user_cache_all()
 *
 *	Invalidate all cache entries in a particular address
 *	space.
 *
 *	- mm	- mm_struct describing address space
 */
ENTRY(v4_flush_user_cache_all)
	/* FALLTHROUGH */
/*
 *	flush_kern_cache_all()
 *
 *	Clean and invalidate the entire cache.
 */
ENTRY(v4_flush_kern_cache_all)
#ifdef CONFIG_CPU_CP15
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0		@ flush ID cache
	mov	pc, lr
#else
	/* FALLTHROUGH */
#endif

/*
 *	flush_user_cache_range(start, end, flags)
 *
 *	Invalidate a range of cache entries in the specified
 *	address space.
 *
 *	- start - start address (may not be aligned)
 *	- end	- end address (exclusive, may not be aligned)
 *	- flags	- vma_area_struct flags describing address space
 */
ENTRY(v4_flush_user_cache_range)
#ifdef CONFIG_CPU_CP15
	mov	ip, #0
<<<<<<< HEAD
	mcreq	p15, 0, ip, c7, c7, 0		@ flush ID cache
=======
	mcr	p15, 0, ip, c7, c7, 0		@ flush ID cache
>>>>>>> refs/remotes/origin/master
	mov	pc, lr
#else
	/* FALLTHROUGH */
#endif

/*
 *	coherent_kern_range(start, end)
 *
 *	Ensure coherency between the Icache and the Dcache in the
 *	region described by start.  If you have non-snooping
 *	Harvard caches, you need to implement this function.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4_coherent_kern_range)
	/* FALLTHROUGH */

/*
 *	coherent_user_range(start, end)
 *
 *	Ensure coherency between the Icache and the Dcache in the
 *	region described by start.  If you have non-snooping
 *	Harvard caches, you need to implement this function.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4_coherent_user_range)
<<<<<<< HEAD
=======
	mov	r0, #0
>>>>>>> refs/remotes/origin/master
	mov	pc, lr

/*
 *	flush_kern_dcache_area(void *addr, size_t size)
 *
 *	Ensure no D cache aliasing occurs, either with itself or
 *	the I cache
 *
 *	- addr	- kernel address
 *	- size	- region size
 */
ENTRY(v4_flush_kern_dcache_area)
	/* FALLTHROUGH */

/*
<<<<<<< HEAD
<<<<<<< HEAD
 *	dma_inv_range(start, end)
 *
 *	Invalidate (discard) the specified virtual address range.
 *	May not write back any entries.  If 'start' or 'end'
 *	are not cache line aligned, those lines must be written
 *	back.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4_dma_inv_range)
	/* FALLTHROUGH */

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *	dma_flush_range(start, end)
 *
 *	Clean and invalidate the specified virtual address range.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4_dma_flush_range)
#ifdef CONFIG_CPU_CP15
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0		@ flush ID cache
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	/* FALLTHROUGH */

/*
 *	dma_clean_range(start, end)
 *
 *	Clean (write back) the specified virtual address range.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4_dma_clean_range)
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mov	pc, lr

/*
 *	dma_unmap_area(start, size, dir)
 *	- start	- kernel virtual start address
 *	- size	- size of region
 *	- dir	- DMA direction
 */
ENTRY(v4_dma_unmap_area)
	teq	r2, #DMA_TO_DEVICE
<<<<<<< HEAD
<<<<<<< HEAD
	bne	v4_dma_inv_range
=======
	bne	v4_dma_flush_range
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bne	v4_dma_flush_range
>>>>>>> refs/remotes/origin/master
	/* FALLTHROUGH */

/*
 *	dma_map_area(start, size, dir)
 *	- start	- kernel virtual start address
 *	- size	- size of region
 *	- dir	- DMA direction
 */
ENTRY(v4_dma_map_area)
	mov	pc, lr
ENDPROC(v4_dma_unmap_area)
ENDPROC(v4_dma_map_area)

<<<<<<< HEAD
	__INITDATA

<<<<<<< HEAD
	.type	v4_cache_fns, #object
ENTRY(v4_cache_fns)
	.long	v4_flush_icache_all
	.long	v4_flush_kern_cache_all
	.long	v4_flush_user_cache_all
	.long	v4_flush_user_cache_range
	.long	v4_coherent_kern_range
	.long	v4_coherent_user_range
	.long	v4_flush_kern_dcache_area
	.long	v4_dma_map_area
	.long	v4_dma_unmap_area
	.long	v4_dma_inv_range
	.long	v4_dma_clean_range
	.long	v4_dma_flush_range
	.size	v4_cache_fns, . - v4_cache_fns
=======
	@ define struct cpu_cache_fns (see <asm/cacheflush.h> and proc-macros.S)
	define_cache_functions v4
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.globl	v4_flush_kern_cache_louis
	.equ	v4_flush_kern_cache_louis, v4_flush_kern_cache_all

	__INITDATA

	@ define struct cpu_cache_fns (see <asm/cacheflush.h> and proc-macros.S)
	define_cache_functions v4
>>>>>>> refs/remotes/origin/master
