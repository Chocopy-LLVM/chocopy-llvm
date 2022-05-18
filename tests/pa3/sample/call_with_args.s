	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"call_with_args.py"
	.globl	before_main
	.p2align	1
	.type	before_main,@function
before_main:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	#APP
	lui	a0, 8192
	mv	s11, a0
	#NO_APP
	call	heap.init@plt
	#APP
	mv	s10, gp
	add	s11, s11, s10
	li	s0, 0
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
	#NO_APP
.Lfunc_end0:
	.size	before_main, .Lfunc_end0-before_main
	.cfi_endproc

	.globl	main
	.p2align	1
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	#APP
	mv	s0, sp
	#NO_APP
	li	a0, 4
	call	($f)@plt
	li	a0, 4
	call	makeint@plt
	call	print@plt
	#APP
	li	a7, 93	#exit system call
	ecall	
	#NO_APP
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.globl	$f
	.p2align	1
	.type	$f,@function
$f:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	sw	s0, 8(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	mv	s0, a0
	lui	a0, %hi(const_9)
	addi	a0, a0, %lo(const_9)
	call	print@plt
	mv	a0, s0
	call	makeint@plt
	call	print@plt
	lui	a0, %hi(const_11)
	addi	a0, a0, %lo(const_11)
	call	print@plt
	li	a0, 1
	call	makeint@plt
	call	print@plt
	mv	a0, s0
	call	makeint@plt
	call	print@plt
	lui	a0, %hi(const_12)
	addi	a0, a0, %lo(const_12)
	call	print@plt
	lui	a0, %hi(const_13)
	addi	a0, a0, %lo(const_13)
	call	print@plt
	lui	a0, %hi(const_10)
	addi	a0, a0, %lo(const_10)
	call	print@plt
	mv	a0, s0
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	$f, .Lfunc_end2-($f)
	.cfi_endproc

	.globl	$g
	.p2align	1
	.type	$g,@function
$g:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	sw	s0, 8(sp)
	sw	s1, 4(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	mv	s0, a1
	mv	s1, a0
	lui	a0, %hi(const_11)
	addi	a0, a0, %lo(const_11)
	call	print@plt
	mv	a0, s1
	call	makeint@plt
	call	print@plt
	mv	a0, s0
	call	makeint@plt
	call	print@plt
	lui	a0, %hi(const_12)
	addi	a0, a0, %lo(const_12)
	call	print@plt
	lui	a0, %hi(const_13)
	addi	a0, a0, %lo(const_13)
	call	print@plt
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	lw	s1, 4(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.size	$g, .Lfunc_end3-($g)
	.cfi_endproc

	.globl	$h
	.p2align	1
	.type	$h,@function
$h:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	call	print@plt
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end4:
	.size	$h, .Lfunc_end4-($h)
	.cfi_endproc

	.type	$object$prototype,@object
	.data
	.globl	$object$prototype
	.p2align	3
$object$prototype:
	.word	0
	.word	3
	.word	($object$dispatchTable)
	.size	$object$prototype, 12

	.type	$object$dispatchTable,@object
	.section	.sdata,"aw",@progbits
	.globl	$object$dispatchTable
	.p2align	3
$object$dispatchTable:
	.word	($object.__init__)
	.size	$object$dispatchTable, 4

	.type	$int$prototype,@object
	.data
	.globl	$int$prototype
	.p2align	3
$int$prototype:
	.word	1
	.word	4
	.word	($int$dispatchTable)
	.word	0
	.size	$int$prototype, 16

	.type	$int$dispatchTable,@object
	.section	.sdata,"aw",@progbits
	.globl	$int$dispatchTable
	.p2align	3
$int$dispatchTable:
	.word	($object.__init__)
	.size	$int$dispatchTable, 4

	.type	$bool$prototype,@object
	.data
	.globl	$bool$prototype
	.p2align	3
$bool$prototype:
	.word	2
	.word	4
	.word	($bool$dispatchTable)
	.byte	0
	.zero	3
	.size	$bool$prototype, 16

	.type	$bool$dispatchTable,@object
	.section	.sdata,"aw",@progbits
	.globl	$bool$dispatchTable
	.p2align	3
$bool$dispatchTable:
	.word	($object.__init__)
	.size	$bool$dispatchTable, 4

	.type	$str$prototype,@object
	.data
	.globl	$str$prototype
	.p2align	4
$str$prototype:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	0
	.word	0
	.size	$str$prototype, 20

	.type	$str$dispatchTable,@object
	.section	.sdata,"aw",@progbits
	.globl	$str$dispatchTable
	.p2align	3
$str$dispatchTable:
	.word	($object.__init__)
	.size	$str$dispatchTable, 4

	.type	$.list$prototype,@object
	.data
	.globl	$.list$prototype
	.p2align	4
$.list$prototype:
	.word	4294967295
	.word	5
	.zero	4
	.word	0
	.word	0
	.size	$.list$prototype, 20

	.type	const_9,@object
	.globl	const_9
	.p2align	4
const_9:
	.word	3
	.word	6
	.word	($str$dispatchTable)
	.word	7
	.word	.Lstr.const_9
	.size	const_9, 20

	.type	.Lstr.const_9,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_9:
	.asciz	"start f"
	.size	.Lstr.const_9, 8

	.type	const_10,@object
	.data
	.globl	const_10
	.p2align	4
const_10:
	.word	3
	.word	6
	.word	($str$dispatchTable)
	.word	5
	.word	.Lstr.const_10
	.size	const_10, 20

	.type	.Lstr.const_10,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_10:
	.asciz	"end f"
	.size	.Lstr.const_10, 6

	.type	const_11,@object
	.data
	.globl	const_11
	.p2align	4
const_11:
	.word	3
	.word	6
	.word	($str$dispatchTable)
	.word	7
	.word	.Lstr.const_11
	.size	const_11, 20

	.type	.Lstr.const_11,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_11:
	.asciz	"start g"
	.size	.Lstr.const_11, 8

	.type	const_12,@object
	.data
	.globl	const_12
	.p2align	4
const_12:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	1
	.word	.Lstr.const_12
	.size	const_12, 20

	.type	.Lstr.const_12,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_12:
	.asciz	"h"
	.size	.Lstr.const_12, 2

	.type	const_13,@object
	.data
	.globl	const_13
	.p2align	4
const_13:
	.word	3
	.word	6
	.word	($str$dispatchTable)
	.word	5
	.word	.Lstr.const_13
	.size	const_13, 20

	.type	.Lstr.const_13,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_13:
	.asciz	"end g"
	.size	.Lstr.const_13, 6

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits












