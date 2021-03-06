/*
 * The head-file for SH-Mobile ARM platforms
 *
 * Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>
 * Simon Horman <horms@verge.net.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef CONFIG_ZBOOT_ROM

	.section	".start", "ax"

	/* load board-specific initialization code */
#include <mach/zboot.h>

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_ZBOOT_ROM_MMCIF
	/* Load image from MMC */
	adr	sp, __tmp_stack + 128
=======
#if defined(CONFIG_ZBOOT_ROM_MMCIF) || defined(CONFIG_ZBOOT_ROM_SH_MOBILE_SDHI)
	/* Load image from MMC/SD */
	adr	sp, __tmp_stack + 256
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(CONFIG_ZBOOT_ROM_MMCIF) || defined(CONFIG_ZBOOT_ROM_SH_MOBILE_SDHI)
	/* Load image from MMC/SD */
	adr	sp, __tmp_stack + 256
>>>>>>> refs/remotes/origin/master
	ldr	r0, __image_start
	ldr	r1, __image_end
	subs	r1, r1, r0
	ldr	r0, __load_base
<<<<<<< HEAD
<<<<<<< HEAD
	bl	mmcif_loader
=======
	bl	mmc_loader
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bl	mmc_loader
>>>>>>> refs/remotes/origin/master

	/* Jump to loaded code */
	ldr	r0, __loaded
	ldr	r1, __image_start
	sub	r0, r0, r1
	ldr	r1, __load_base
	add	pc, r0, r1

__image_start:
	.long	_start
__image_end:
	.long	_got_end
__load_base:
<<<<<<< HEAD
	.long	CONFIG_MEMORY_START + 0x02000000 @ Load at 32Mb into SDRAM
=======
	.long	MEMORY_START + 0x02000000 @ Load at 32Mb into SDRAM
>>>>>>> refs/remotes/origin/master
__loaded:
	.long	__continue
	.align
__tmp_stack:
<<<<<<< HEAD
<<<<<<< HEAD
	.space	128
__continue:
#endif /* CONFIG_ZBOOT_ROM_MMCIF */
=======
	.space	256
__continue:
#endif /* CONFIG_ZBOOT_ROM_MMC || CONFIG_ZBOOT_ROM_SH_MOBILE_SDHI */
>>>>>>> refs/remotes/origin/cm-10.0

	b	1f
__atags:@ tag #1
	.long	12			@ tag->hdr.size = tag_size(tag_core);
	.long	0x54410001		@ tag->hdr.tag = ATAG_CORE;
	.long   0			@ tag->u.core.flags = 0;
	.long	0			@ tag->u.core.pagesize = 0;
	.long	0			@ tag->u.core.rootdev = 0;
	@ tag #2
	.long	8			@ tag->hdr.size = tag_size(tag_mem32);
	.long	0x54410002		@ tag->hdr.tag = ATAG_MEM;
	.long	CONFIG_MEMORY_SIZE	@ tag->u.mem.size = CONFIG_MEMORY_SIZE;
	.long	CONFIG_MEMORY_START	@ @ tag->u.mem.start = CONFIG_MEMORY_START;
	@ tag #3
	.long	0			@ tag->hdr.size = 0
	.long	0			@ tag->hdr.tag = ATAG_NONE;
1:

	/* Set board ID necessary for boot */
	ldr	r7, 1f				@ Set machine type register
	adr	r8, __atags			@ Set atag register
	b	2f

1 :	.long MACH_TYPE
=======
	.space	256
__continue:
#endif /* CONFIG_ZBOOT_ROM_MMC || CONFIG_ZBOOT_ROM_SH_MOBILE_SDHI */

	adr	r0, dtb_info
	ldmia	r0, {r1, r3, r4, r5, r7}

	sub	r0, r0, r1		@ calculate the delta offset
	add	r5, r5, r0		@ _edata

	ldr	lr, [r5, #0]		@ check if valid DTB is present
	cmp	lr, r3
	bne	0f

	add	r9, r7, #31		@ rounded up to a multiple
	bic	r9, r9, #31		@ ... of 32 bytes

	add	r6, r9, r5		@ copy from _edata
	add	r9, r9, r4		@ to MEMORY_START

1:	ldmdb	r6!, {r0 - r3, r10 - r12, lr}
	cmp	r6, r5
	stmdb	r9!, {r0 - r3, r10 - r12, lr}
	bhi	1b

	/* Success: Zero board ID, pointer to start of memory for atag/dtb */
	mov	r7, #0
	mov	r8, r4
	b	2f

	.align	2
dtb_info:
	.word	dtb_info
#ifndef __ARMEB__
	.word	0xedfe0dd0		@ sig is 0xd00dfeed big endian
#else
	.word	0xd00dfeed
#endif
	.word	MEMORY_START
	.word	_edata
	.word	0x4000			@ maximum DTB size
0:
	/* Failure: Zero board ID, NULL atag/dtb */
	mov 	r7, #0
	mov	r8, #0			@ pass null pointer as atag
>>>>>>> refs/remotes/origin/master
2 :

#endif /* CONFIG_ZBOOT_ROM */
