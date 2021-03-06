/*
 * The ARM LDRD and Thumb LDRSB instructions use bit 20/11 (ARM/Thumb)
 * differently than every other instruction, so it is set to 0 (write)
 * even though the instructions are read instructions. This means that
 * during an abort the instructions will be treated as a write and the
 * handler will raise a signal from unwriteable locations if they
 * fault. We have to specifically check for these instructions
 * from the abort handlers to treat them properly.
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
	.macro	do_thumb_abort
	tst	r3, #PSR_T_BIT
	beq	not_thumb
	ldrh	r3, [r2]			@ Read aborted Thumb instruction
	and	r3, r3, # 0xfe00		@ Mask opcode field
	cmp	r3, # 0x5600			@ Is it ldrsb?
	orreq	r3, r3, #1 << 11		@ Set L-bit if yes
	tst	r3, #1 << 11			@ L = 0 -> write
	orreq	r1, r1, #1 << 11		@ yes.
	mov	pc, lr
=======
=======
>>>>>>> refs/remotes/origin/master
	.macro	do_thumb_abort, fsr, pc, psr, tmp
	tst	\psr, #PSR_T_BIT
	beq	not_thumb
	ldrh	\tmp, [\pc]			@ Read aborted Thumb instruction
	and	\tmp, \tmp, # 0xfe00		@ Mask opcode field
	cmp	\tmp, # 0x5600			@ Is it ldrsb?
	orreq	\tmp, \tmp, #1 << 11		@ Set L-bit if yes
	tst	\tmp, #1 << 11			@ L = 0 -> write
	orreq	\fsr, \fsr, #1 << 11		@ yes.
	b	do_DataAbort
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
not_thumb:
	.endm

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * We check for the following insturction encoding for LDRD.
 *
 * [27:25] == 0
 *   [7:4] == 1101
 *    [20] == 0
 */
 	.macro	do_ldrd_abort
 	tst	r3, #0x0e000000			@ [27:25] == 0
	bne	not_ldrd
	and	r2, r3, #0x000000f0		@ [7:4] == 1101
	cmp	r2, #0x000000d0
	bne	not_ldrd
	tst	r3, #1 << 20			@ [20] == 0
	moveq	pc, lr
=======
=======
>>>>>>> refs/remotes/origin/master
 * We check for the following instruction encoding for LDRD.
 *
 * [27:25] == 000
 *   [7:4] == 1101
 *    [20] == 0
 */
	.macro	do_ldrd_abort, tmp, insn
	tst	\insn, #0x0e100000		@ [27:25,20] == 0
	bne	not_ldrd
	and	\tmp, \insn, #0x000000f0	@ [7:4] == 1101
	cmp	\tmp, #0x000000d0
	beq	do_DataAbort
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
not_ldrd:
	.endm

