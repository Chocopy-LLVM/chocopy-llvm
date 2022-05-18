	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"str_cat.py"
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
	lui	a0, %hi(const_9)
	addi	a0, a0, %lo(const_9)
	sw	a0, 8(sp)
	lui	a1, %hi(const_10)
	addi	a1, a1, %lo(const_10)
	call	($cat2)@plt
	call	print@plt
	lui	a0, %hi(const_12)
	addi	a0, a0, %lo(const_12)
	lui	a1, %hi(const_11)
	addi	a1, a1, %lo(const_11)
	sw	a1, 4(sp)
	call	($cat2)@plt
	call	print@plt
	lw	a2, 4(sp)
	lw	a0, 8(sp)
	lui	a1, %hi(const_13)
	addi	a1, a1, %lo(const_13)
	call	($cat3)@plt
	call	print@plt
	lw	a0, 8(sp)
	call	($len)@plt
	call	makeint@plt
	call	print@plt
	lw	a1, 8(sp)
	mv	a0, a1
	call	($cat2)@plt
	call	($len)@plt
	call	makeint@plt
	call	print@plt
	lui	a0, %hi(const_14)
	addi	a0, a0, %lo(const_14)
	lui	a1, %hi(const_15)
	addi	a1, a1, %lo(const_15)
	call	($cat2)@plt
	call	($len)@plt
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

	.globl	$cat2
	.p2align	1
	.type	$cat2,@function
$cat2:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	call	strcat@plt
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	$cat2, .Lfunc_end2-($cat2)
	.cfi_endproc

	.globl	$cat3
	.p2align	1
	.type	$cat3,@function
$cat3:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	sw	a2, 8(sp)
	call	strcat@plt
	lw	a1, 8(sp)
	call	strcat@plt
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.size	$cat3, .Lfunc_end3-($cat3)
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
	.word	5
	.word	.Lstr.const_9
	.size	const_9, 20

	.type	.Lstr.const_9,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_9:
	.asciz	"Hello"
	.size	.Lstr.const_9, 6

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
	.asciz	"World"
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
	.asciz	"ChocoPy"
	.size	.Lstr.const_11, 8

	.type	const_12,@object
	.data
	.globl	const_12
	.p2align	4
const_12:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	0
	.word	.Lstr.const_12
	.size	const_12, 20

	.type	.Lstr.const_12,@object
	.section	.sbss,"aw",@nobits
.Lstr.const_12:
	.zero	1
	.size	.Lstr.const_12, 1

	.type	const_13,@object
	.data
	.globl	const_13
	.p2align	4
const_13:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	1
	.word	.Lstr.const_13
	.size	const_13, 20

	.type	.Lstr.const_13,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_13:
	.asciz	" "
	.size	.Lstr.const_13, 2

	.type	const_14,@object
	.data
	.globl	const_14
	.p2align	4
const_14:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	0
	.word	.Lstr.const_14
	.size	const_14, 20

	.type	.Lstr.const_14,@object
	.section	.sbss,"aw",@nobits
.Lstr.const_14:
	.zero	1
	.size	.Lstr.const_14, 1

	.type	const_15,@object
	.data
	.globl	const_15
	.p2align	4
const_15:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	0
	.word	.Lstr.const_15
	.size	const_15, 20

	.type	.Lstr.const_15,@object
	.section	.sbss,"aw",@nobits
.Lstr.const_15:
	.zero	1
	.size	.Lstr.const_15, 1

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits





















