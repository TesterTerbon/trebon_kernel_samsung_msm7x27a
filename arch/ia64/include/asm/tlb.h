#ifndef _ASM_IA64_TLB_H
#define _ASM_IA64_TLB_H
/*
 * Based on <asm-generic/tlb.h>.
 *
 * Copyright (C) 2002-2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */
/*
 * Removing a translation from a page table (including TLB-shootdown) is a four-step
 * procedure:
 *
 *	(1) Flush (virtual) caches --- ensures virtual memory is coherent with kernel memory
 *	    (this is a no-op on ia64).
 *	(2) Clear the relevant portions of the page-table
 *	(3) Flush the TLBs --- ensures that stale content is gone from CPU TLBs
 *	(4) Release the pages that were freed up in step (2).
 *
 * Note that the ordering of these steps is crucial to avoid races on MP machines.
 *
 * The Linux kernel defines several platform-specific hooks for TLB-shootdown.  When
 * unmapping a portion of the virtual address space, these hooks are called according to
 * the following template:
 *
<<<<<<< HEAD
 *	tlb <- tlb_gather_mmu(mm, full_mm_flush);	// start unmap for address space MM
=======
 *	tlb <- tlb_gather_mmu(mm, start, end);		// start unmap for address space MM
>>>>>>> refs/remotes/origin/master
 *	{
 *	  for each vma that needs a shootdown do {
 *	    tlb_start_vma(tlb, vma);
 *	      for each page-table-entry PTE that needs to be removed do {
 *		tlb_remove_tlb_entry(tlb, pte, address);
 *		if (pte refers to a normal page) {
 *		  tlb_remove_page(tlb, page);
 *		}
 *	      }
 *	    tlb_end_vma(tlb, vma);
 *	  }
 *	}
 *	tlb_finish_mmu(tlb, start, end);	// finish unmap for address space MM
 */
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/swap.h>

#include <asm/pgalloc.h>
#include <asm/processor.h>
#include <asm/tlbflush.h>
#include <asm/machvec.h>

<<<<<<< HEAD
#ifdef CONFIG_SMP
# define tlb_fast_mode(tlb)	((tlb)->nr == ~0U)
#else
# define tlb_fast_mode(tlb)	(1)
#endif

=======
>>>>>>> refs/remotes/origin/master
/*
 * If we can't allocate a page to make a big batch of page pointers
 * to work on, then just handle a few from the on-stack structure.
 */
#define	IA64_GATHER_BUNDLE	8

struct mmu_gather {
	struct mm_struct	*mm;
<<<<<<< HEAD
	unsigned int		nr;		/* == ~0U => fast mode */
	unsigned int		max;
	unsigned char		fullmm;		/* non-zero means full mm flush */
	unsigned char		need_flush;	/* really unmapped some PTEs? */
=======
	unsigned int		nr;
	unsigned int		max;
	unsigned char		fullmm;		/* non-zero means full mm flush */
	unsigned char		need_flush;	/* really unmapped some PTEs? */
	unsigned long		start, end;
>>>>>>> refs/remotes/origin/master
	unsigned long		start_addr;
	unsigned long		end_addr;
	struct page		**pages;
	struct page		*local[IA64_GATHER_BUNDLE];
};

struct ia64_tr_entry {
	u64 ifa;
	u64 itir;
	u64 pte;
	u64 rr;
}; /*Record for tr entry!*/

extern int ia64_itr_entry(u64 target_mask, u64 va, u64 pte, u64 log_size);
extern void ia64_ptr_entry(u64 target_mask, int slot);

extern struct ia64_tr_entry *ia64_idtrs[NR_CPUS];

/*
 region register macros
*/
#define RR_TO_VE(val)   (((val) >> 0) & 0x0000000000000001)
#define RR_VE(val)	(((val) & 0x0000000000000001) << 0)
#define RR_VE_MASK	0x0000000000000001L
#define RR_VE_SHIFT	0
#define RR_TO_PS(val)	(((val) >> 2) & 0x000000000000003f)
#define RR_PS(val)	(((val) & 0x000000000000003f) << 2)
#define RR_PS_MASK	0x00000000000000fcL
#define RR_PS_SHIFT	2
#define RR_RID_MASK	0x00000000ffffff00L
#define RR_TO_RID(val) 	((val >> 8) & 0xffffff)

/*
 * Flush the TLB for address range START to END and, if not in fast mode, release the
 * freed pages that where gathered up to this point.
 */
static inline void
ia64_tlb_flush_mmu (struct mmu_gather *tlb, unsigned long start, unsigned long end)
{
<<<<<<< HEAD
=======
	unsigned long i;
>>>>>>> refs/remotes/origin/master
	unsigned int nr;

	if (!tlb->need_flush)
		return;
	tlb->need_flush = 0;

	if (tlb->fullmm) {
		/*
		 * Tearing down the entire address space.  This happens both as a result
		 * of exit() and execve().  The latter case necessitates the call to
		 * flush_tlb_mm() here.
		 */
		flush_tlb_mm(tlb->mm);
	} else if (unlikely (end - start >= 1024*1024*1024*1024UL
			     || REGION_NUMBER(start) != REGION_NUMBER(end - 1)))
	{
		/*
		 * If we flush more than a tera-byte or across regions, we're probably
		 * better off just flushing the entire TLB(s).  This should be very rare
		 * and is not worth optimizing for.
		 */
		flush_tlb_all();
	} else {
		/*
		 * XXX fix me: flush_tlb_range() should take an mm pointer instead of a
		 * vma pointer.
		 */
		struct vm_area_struct vma;

		vma.vm_mm = tlb->mm;
		/* flush the address range from the tlb: */
		flush_tlb_range(&vma, start, end);
		/* now flush the virt. page-table area mapping the address range: */
		flush_tlb_range(&vma, ia64_thash(start), ia64_thash(end));
	}

	/* lastly, release the freed pages */
	nr = tlb->nr;
<<<<<<< HEAD
	if (!tlb_fast_mode(tlb)) {
		unsigned long i;
		tlb->nr = 0;
		tlb->start_addr = ~0UL;
		for (i = 0; i < nr; ++i)
			free_page_and_swap_cache(tlb->pages[i]);
	}
=======

	tlb->nr = 0;
	tlb->start_addr = ~0UL;
	for (i = 0; i < nr; ++i)
		free_page_and_swap_cache(tlb->pages[i]);
>>>>>>> refs/remotes/origin/master
}

static inline void __tlb_alloc_page(struct mmu_gather *tlb)
{
	unsigned long addr = __get_free_pages(GFP_NOWAIT | __GFP_NOWARN, 0);

	if (addr) {
		tlb->pages = (void *)addr;
		tlb->max = PAGE_SIZE / sizeof(void *);
	}
}


static inline void
<<<<<<< HEAD
tlb_gather_mmu(struct mmu_gather *tlb, struct mm_struct *mm, unsigned int full_mm_flush)
=======
tlb_gather_mmu(struct mmu_gather *tlb, struct mm_struct *mm, unsigned long start, unsigned long end)
>>>>>>> refs/remotes/origin/master
{
	tlb->mm = mm;
	tlb->max = ARRAY_SIZE(tlb->local);
	tlb->pages = tlb->local;
<<<<<<< HEAD
	/*
	 * Use fast mode if only 1 CPU is online.
	 *
	 * It would be tempting to turn on fast-mode for full_mm_flush as well.  But this
	 * doesn't work because of speculative accesses and software prefetching: the page
	 * table of "mm" may (and usually is) the currently active page table and even
	 * though the kernel won't do any user-space accesses during the TLB shoot down, a
	 * compiler might use speculation or lfetch.fault on what happens to be a valid
	 * user-space address.  This in turn could trigger a TLB miss fault (or a VHPT
	 * walk) and re-insert a TLB entry we just removed.  Slow mode avoids such
	 * problems.  (We could make fast-mode work by switching the current task to a
	 * different "mm" during the shootdown.) --davidm 08/02/2002
	 */
	tlb->nr = (num_online_cpus() == 1) ? ~0U : 0;
	tlb->fullmm = full_mm_flush;
=======
	tlb->nr = 0;
	tlb->fullmm = !(start | (end+1));
	tlb->start = start;
	tlb->end = end;
>>>>>>> refs/remotes/origin/master
	tlb->start_addr = ~0UL;
}

/*
 * Called at the end of the shootdown operation to free up any resources that were
 * collected.
 */
static inline void
tlb_finish_mmu(struct mmu_gather *tlb, unsigned long start, unsigned long end)
{
	/*
	 * Note: tlb->nr may be 0 at this point, so we can't rely on tlb->start_addr and
	 * tlb->end_addr.
	 */
	ia64_tlb_flush_mmu(tlb, start, end);

	/* keep the page table cache within bounds */
	check_pgt_cache();

	if (tlb->pages != tlb->local)
		free_pages((unsigned long)tlb->pages, 0);
}

/*
 * Logically, this routine frees PAGE.  On MP machines, the actual freeing of the page
 * must be delayed until after the TLB has been flushed (see comments at the beginning of
 * this file).
 */
static inline int __tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	tlb->need_flush = 1;

<<<<<<< HEAD
	if (tlb_fast_mode(tlb)) {
		free_page_and_swap_cache(page);
		return 1; /* avoid calling tlb_flush_mmu */
	}

=======
>>>>>>> refs/remotes/origin/master
	if (!tlb->nr && tlb->pages == tlb->local)
		__tlb_alloc_page(tlb);

	tlb->pages[tlb->nr++] = page;
	VM_BUG_ON(tlb->nr > tlb->max);

	return tlb->max - tlb->nr;
}

static inline void tlb_flush_mmu(struct mmu_gather *tlb)
{
	ia64_tlb_flush_mmu(tlb, tlb->start_addr, tlb->end_addr);
}

static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	if (!__tlb_remove_page(tlb, page))
		tlb_flush_mmu(tlb);
}

/*
 * Remove TLB entry for PTE mapped at virtual address ADDRESS.  This is called for any
 * PTE, not just those pointing to (normal) physical memory.
 */
static inline void
__tlb_remove_tlb_entry (struct mmu_gather *tlb, pte_t *ptep, unsigned long address)
{
	if (tlb->start_addr == ~0UL)
		tlb->start_addr = address;
	tlb->end_addr = address + PAGE_SIZE;
}

#define tlb_migrate_finish(mm)	platform_tlb_migrate_finish(mm)

#define tlb_start_vma(tlb, vma)			do { } while (0)
#define tlb_end_vma(tlb, vma)			do { } while (0)

#define tlb_remove_tlb_entry(tlb, ptep, addr)		\
do {							\
	tlb->need_flush = 1;				\
	__tlb_remove_tlb_entry(tlb, ptep, addr);	\
} while (0)

#define pte_free_tlb(tlb, ptep, address)		\
do {							\
	tlb->need_flush = 1;				\
	__pte_free_tlb(tlb, ptep, address);		\
} while (0)

#define pmd_free_tlb(tlb, ptep, address)		\
do {							\
	tlb->need_flush = 1;				\
	__pmd_free_tlb(tlb, ptep, address);		\
} while (0)

#define pud_free_tlb(tlb, pudp, address)		\
do {							\
	tlb->need_flush = 1;				\
	__pud_free_tlb(tlb, pudp, address);		\
} while (0)

#endif /* _ASM_IA64_TLB_H */
