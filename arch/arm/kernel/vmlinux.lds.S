/* ld script to make ARM Linux kernel
 * taken from the i386 version by Russell King
 * Written by Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 */

#include <asm-generic/vmlinux.lds.h>
#include <asm/cache.h>
#include <asm/thread_info.h>
#include <asm/memory.h>
#include <asm/page.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_STRICT_MEMORY_RWX
#include <asm/pgtable.h>
#endif

<<<<<<< HEAD
#define PROC_INFO							\
<<<<<<< HEAD
=======
	. = ALIGN(4);							\
>>>>>>> refs/remotes/origin/cm-10.0
=======
	
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define PROC_INFO							\
	. = ALIGN(4);							\
>>>>>>> refs/remotes/origin/master
	VMLINUX_SYMBOL(__proc_info_begin) = .;				\
	*(.proc.info.init)						\
	VMLINUX_SYMBOL(__proc_info_end) = .;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define IDMAP_TEXT							\
	ALIGN_FUNCTION();						\
	VMLINUX_SYMBOL(__idmap_text_start) = .;				\
	*(.idmap.text)							\
<<<<<<< HEAD
	VMLINUX_SYMBOL(__idmap_text_end) = .;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	VMLINUX_SYMBOL(__idmap_text_end) = .;				\
	. = ALIGN(32);							\
	VMLINUX_SYMBOL(__hyp_idmap_text_start) = .;			\
	*(.hyp.idmap.text)						\
	VMLINUX_SYMBOL(__hyp_idmap_text_end) = .;

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_HOTPLUG_CPU
#define ARM_CPU_DISCARD(x)
#define ARM_CPU_KEEP(x)		x
#else
#define ARM_CPU_DISCARD(x)	x
#define ARM_CPU_KEEP(x)
#endif

#if (defined(CONFIG_SMP_ON_UP) && !defined(CONFIG_DEBUG_SPINLOCK)) || \
	defined(CONFIG_GENERIC_BUG)
#define ARM_EXIT_KEEP(x)	x
<<<<<<< HEAD
<<<<<<< HEAD
#else
#define ARM_EXIT_KEEP(x)
=======
=======
>>>>>>> refs/remotes/origin/master
#define ARM_EXIT_DISCARD(x)
#else
#define ARM_EXIT_KEEP(x)
#define ARM_EXIT_DISCARD(x)	x
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif

OUTPUT_ARCH(arm)
ENTRY(stext)

#ifndef __ARMEB__
jiffies = jiffies_64;
#else
jiffies = jiffies_64 + 4;
#endif

SECTIONS
{
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_XIP_KERNEL
	. = XIP_VIRT_ADDR(CONFIG_XIP_PHYS_ADDR);
#else
	. = PAGE_OFFSET + TEXT_OFFSET;
#endif

	.init : {			/* Init code and data		*/
		_stext = .;
		_sinittext = .;
			HEAD_TEXT
			INIT_TEXT
			ARM_EXIT_KEEP(EXIT_TEXT)
		_einittext = .;
		ARM_CPU_DISCARD(PROC_INFO)
		__arch_info_begin = .;
			*(.arch.info.init)
		__arch_info_end = .;
		__tagtable_begin = .;
			*(.taglist.init)
		__tagtable_end = .;
#ifdef CONFIG_SMP_ON_UP
		__smpalt_begin = .;
			*(.alt.smp.init)
		__smpalt_end = .;
#endif

		__pv_table_begin = .;
			*(.pv_table)
		__pv_table_end = .;

		INIT_SETUP(16)

		INIT_CALLS
		CON_INITCALL
		SECURITY_INITCALL
		INIT_RAM_FS

#ifndef CONFIG_XIP_KERNEL
		__init_begin = _stext;
		INIT_DATA
		ARM_EXIT_KEEP(EXIT_DATA)
#endif
	}

	PERCPU_SECTION(L1_CACHE_BYTES)

#ifndef CONFIG_XIP_KERNEL
	. = ALIGN(PAGE_SIZE);
	__init_end = .;
#endif

	/*
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * XXX: The linker does not define how output sections are
	 * assigned to input sections when there are multiple statements
	 * matching the same input section name.  There is no documented
	 * order of matching.
	 *
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 * unwind exit sections must be discarded before the rest of the
	 * unwind sections get included.
	 */
	/DISCARD/ : {
		*(.ARM.exidx.exit.text)
		*(.ARM.extab.exit.text)
		ARM_CPU_DISCARD(*(.ARM.exidx.cpuexit.text))
		ARM_CPU_DISCARD(*(.ARM.extab.cpuexit.text))
<<<<<<< HEAD
<<<<<<< HEAD
=======
		ARM_EXIT_DISCARD(EXIT_TEXT)
		ARM_EXIT_DISCARD(EXIT_DATA)
		EXIT_CALL
>>>>>>> refs/remotes/origin/cm-10.0
#ifndef CONFIG_HOTPLUG
		*(.ARM.exidx.devexit.text)
		*(.ARM.extab.devexit.text)
#endif
=======
		ARM_EXIT_DISCARD(EXIT_TEXT)
		ARM_EXIT_DISCARD(EXIT_DATA)
		EXIT_CALL
>>>>>>> refs/remotes/origin/master
#ifndef CONFIG_MMU
		*(.fixup)
		*(__ex_table)
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	}

#ifdef CONFIG_STRICT_MEMORY_RWX
	. = ALIGN(1<<SECTION_SHIFT);
#endif
	.text : {			/* Real text segment		*/
		_text = .;		/* Text and read-only data	*/
=======
=======
>>>>>>> refs/remotes/origin/master
#ifndef CONFIG_SMP_ON_UP
		*(.alt.smp.init)
#endif
		*(.discard)
		*(.discard.*)
	}

#ifdef CONFIG_XIP_KERNEL
	. = XIP_VIRT_ADDR(CONFIG_XIP_PHYS_ADDR);
#else
	. = PAGE_OFFSET + TEXT_OFFSET;
#endif
	.head.text : {
		_text = .;
		HEAD_TEXT
	}
<<<<<<< HEAD
<<<<<<< HEAD

	.text : {			/* Real text segment		*/
		_stext = .;		/* Text and read-only data	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
=======

>>>>>>> refs/remotes/origin/cm-11.0
	.text : {			/* Real text segment		*/
		_stext = .;		/* Text and read-only data	*/
>>>>>>> refs/remotes/origin/master
			__exception_text_start = .;
			*(.exception.text)
			__exception_text_end = .;
			IRQENTRY_TEXT
			TEXT_TEXT
			SCHED_TEXT
			LOCK_TEXT
			KPROBES_TEXT
<<<<<<< HEAD
<<<<<<< HEAD
=======
			IDMAP_TEXT
>>>>>>> refs/remotes/origin/cm-10.0
=======
			IDMAP_TEXT
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_MMU
			*(.fixup)
#endif
			*(.gnu.warning)
<<<<<<< HEAD
<<<<<<< HEAD
			*(.rodata)
			*(.rodata.*)
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			*(.glue_7)
			*(.glue_7t)
		. = ALIGN(4);
		*(.got)			/* Global offset table		*/
			ARM_CPU_KEEP(PROC_INFO)
	}
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_STRICT_MEMORY_RWX
	. = ALIGN(1<<SECTION_SHIFT);
#endif

	RO_DATA(PAGE_SIZE)

=======
=======
#ifdef CONFIG_STRICT_MEMORY_RWX
	. = ALIGN(1<<SECTION_SHIFT);
#endif
>>>>>>> refs/remotes/origin/cm-11.0

	RO_DATA(PAGE_SIZE)

	. = ALIGN(4);
	__ex_table : AT(ADDR(__ex_table) - LOAD_OFFSET) {
		__start___ex_table = .;
#ifdef CONFIG_MMU
		*(__ex_table)
#endif
		__stop___ex_table = .;
	}

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_ARM_UNWIND
	/*
	 * Stack unwinding tables
	 */
	. = ALIGN(8);
	.ARM.unwind_idx : {
		__start_unwind_idx = .;
		*(.ARM.exidx*)
		__stop_unwind_idx = .;
	}
	.ARM.unwind_tab : {
		__start_unwind_tab = .;
		*(.ARM.extab*)
		__stop_unwind_tab = .;
	}
#endif

<<<<<<< HEAD
	_etext = .;			/* End of text and rodata section */

<<<<<<< HEAD
#ifdef CONFIG_XIP_KERNEL
	__data_loc = ALIGN(4);		/* location in binary */
	. = PAGE_OFFSET + TEXT_OFFSET;
#else
#ifdef CONFIG_STRICT_MEMORY_RWX
	. = ALIGN(1<<SECTION_SHIFT);
#else
	. = ALIGN(THREAD_SIZE);
#endif
=======
#ifndef CONFIG_XIP_KERNEL
#ifdef CONFIG_STRICT_MEMORY_RWX
	. = ALIGN(1<<SECTION_SHIFT);
#else
	. = ALIGN(PAGE_SIZE);
#endif
	__init_begin = .;
#endif
=======
	NOTES

	_etext = .;			/* End of text and rodata section */

#ifndef CONFIG_XIP_KERNEL
	. = ALIGN(PAGE_SIZE);
	__init_begin = .;
#endif
	/*
	 * The vectors and stubs are relocatable code, and the
	 * only thing that matters is their relative offsets
	 */
	__vectors_start = .;
	.vectors 0 : AT(__vectors_start) {
		*(.vectors)
	}
	. = __vectors_start + SIZEOF(.vectors);
	__vectors_end = .;

	__stubs_start = .;
	.stubs 0x1000 : AT(__stubs_start) {
		*(.stubs)
	}
	. = __stubs_start + SIZEOF(.stubs);
	__stubs_end = .;
>>>>>>> refs/remotes/origin/master

	INIT_TEXT_SECTION(8)
	.exit.text : {
		ARM_EXIT_KEEP(EXIT_TEXT)
	}
	.init.proc.info : {
		ARM_CPU_DISCARD(PROC_INFO)
	}
	.init.arch.info : {
		__arch_info_begin = .;
		*(.arch.info.init)
		__arch_info_end = .;
	}
	.init.tagtable : {
		__tagtable_begin = .;
		*(.taglist.init)
		__tagtable_end = .;
	}
#ifdef CONFIG_SMP_ON_UP
	.init.smpalt : {
		__smpalt_begin = .;
		*(.alt.smp.init)
		__smpalt_end = .;
	}
#endif
	.init.pv_table : {
		__pv_table_begin = .;
		*(.pv_table)
		__pv_table_end = .;
	}
	.init.data : {
#ifndef CONFIG_XIP_KERNEL
		INIT_DATA
#endif
		INIT_SETUP(16)
		INIT_CALLS
		CON_INITCALL
		SECURITY_INITCALL
		INIT_RAM_FS
	}
#ifndef CONFIG_XIP_KERNEL
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_STRICT_MEMORY_RWX
	. = ALIGN(1<<SECTION_SHIFT);
#endif
	__init_data = .;
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.exit.data : {
		ARM_EXIT_KEEP(EXIT_DATA)
	}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#if !defined(CONFIG_SMP) && !defined(CONFIG_XIP_KERNEL)
	. = ALIGN(THREAD_SIZE);
#endif
	PERCPU_SECTION(L1_CACHE_BYTES)
=======
#ifdef CONFIG_SMP
=======
#if !defined(CONFIG_SMP) && !defined(CONFIG_XIP_KERNEL)
	. = ALIGN(THREAD_SIZE);
#endif
>>>>>>> refs/remotes/origin/cm-11.0
	PERCPU_SECTION(L1_CACHE_BYTES)
#endif
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_XIP_KERNEL
	__data_loc = ALIGN(4);		/* location in binary */
	. = PAGE_OFFSET + TEXT_OFFSET;
#else
	__init_end = .;
	. = ALIGN(THREAD_SIZE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	__data_loc = .;
#endif

	.data : AT(__data_loc) {
		_data = .;		/* address in memory */
		_sdata = .;

		/*
		 * first, the init task union, aligned
		 * to an 8192 byte boundary.
		 */
		INIT_TASK_DATA(THREAD_SIZE)

#ifdef CONFIG_XIP_KERNEL
		. = ALIGN(PAGE_SIZE);
		__init_begin = .;
		INIT_DATA
		ARM_EXIT_KEEP(EXIT_DATA)
		. = ALIGN(PAGE_SIZE);
		__init_end = .;
#endif

		NOSAVE_DATA
		CACHELINE_ALIGNED_DATA(L1_CACHE_BYTES)
		READ_MOSTLY_DATA(L1_CACHE_BYTES)

		/*
<<<<<<< HEAD
		 * The exception fixup table (might need resorting at runtime)
		 */
		. = ALIGN(L1_CACHE_BYTES);
		__start___ex_table = .;
#ifdef CONFIG_MMU
		*(__ex_table)
#endif
		__stop___ex_table = .;

		/*
=======
>>>>>>> refs/remotes/origin/master
		 * and the usual data section
		 */
		DATA_DATA
		CONSTRUCTORS

		_edata = .;
	}
	_edata_loc = __data_loc + SIZEOF(.data);

#ifdef CONFIG_HAVE_TCM
        /*
	 * We align everything to a page boundary so we can
	 * free it after init has commenced and TCM contents have
	 * been copied to its destination.
	 */
	.tcm_start : {
		. = ALIGN(PAGE_SIZE);
		__tcm_start = .;
		__itcm_start = .;
	}

	/*
	 * Link these to the ITCM RAM
	 * Put VMA to the TCM address and LMA to the common RAM
	 * and we'll upload the contents from RAM to TCM and free
	 * the used RAM after that.
	 */
	.text_itcm ITCM_OFFSET : AT(__itcm_start)
	{
		__sitcm_text = .;
		*(.tcm.text)
		*(.tcm.rodata)
		. = ALIGN(4);
		__eitcm_text = .;
	}

	/*
	 * Reset the dot pointer, this is needed to create the
	 * relative __dtcm_start below (to be used as extern in code).
	 */
	. = ADDR(.tcm_start) + SIZEOF(.tcm_start) + SIZEOF(.text_itcm);

	.dtcm_start : {
		__dtcm_start = .;
	}

	/* TODO: add remainder of ITCM as well, that can be used for data! */
	.data_dtcm DTCM_OFFSET : AT(__dtcm_start)
	{
		. = ALIGN(4);
		__sdtcm_data = .;
		*(.tcm.data)
		. = ALIGN(4);
		__edtcm_data = .;
	}

	/* Reset the dot pointer or the linker gets confused */
	. = ADDR(.dtcm_start) + SIZEOF(.data_dtcm);

	/* End marker for freeing TCM copy in linked object */
	.tcm_end : AT(ADDR(.dtcm_start) + SIZEOF(.data_dtcm)){
		. = ALIGN(PAGE_SIZE);
		__tcm_end = .;
	}
#endif

<<<<<<< HEAD
	NOTES

=======
>>>>>>> refs/remotes/origin/master
	BSS_SECTION(0, 0, 0)
	_end = .;

	STABS_DEBUG
	.comment 0 : { *(.comment) }
<<<<<<< HEAD
<<<<<<< HEAD

	/* Default discards */
	DISCARDS

#ifndef CONFIG_SMP_ON_UP
	/DISCARD/ : {
		*(.alt.smp.init)
	}
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * These must never be empty
 * If you have to comment these two assert statements out, your
 * binutils is too old (for other reasons as well)
 */
ASSERT((__proc_info_end - __proc_info_begin), "missing CPU support")
ASSERT((__arch_info_end - __arch_info_begin), "no machine record defined")
<<<<<<< HEAD
=======
/*
 * The HYP init code can't be more than a page long.
 * The above comment applies as well.
 */
ASSERT(((__hyp_idmap_text_end - __hyp_idmap_text_start) <= PAGE_SIZE), "HYP init code too big")
>>>>>>> refs/remotes/origin/master
