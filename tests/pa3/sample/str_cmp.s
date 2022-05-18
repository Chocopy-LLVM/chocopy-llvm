	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"str_cmp.py"
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
	sw	s0, 8(sp)
	sw	s1, 4(sp)
	sw	s2, 0(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	.cfi_offset s2, -16
	#APP
	mv	s0, sp
	#NO_APP
	lui	a0, %hi(const_9)
	addi	s2, a0, %lo(const_9)
	mv	a0, s2
	mv	a1, s2
	call	streql@plt
	call	makebool@plt
	call	print@plt
	lui	a0, %hi(const_10)
	addi	s1, a0, %lo(const_10)
	mv	a0, s2
	mv	a1, s1
	call	streql@plt
	call	makebool@plt
	call	print@plt
	mv	a0, s2
	mv	a1, s1
	call	strneql@plt
	call	makebool@plt
	call	print@plt
	mv	a0, s1
	mv	a1, s1
	call	strneql@plt
	call	makebool@plt
	call	print@plt
	lui	a0, %hi(const_11)
	addi	s0, a0, %lo(const_11)
	mv	a0, s0
	mv	a1, s2
	call	streql@plt
	call	makebool@plt
	call	print@plt
	mv	a0, s0
	mv	a1, s1
	call	strneql@plt
	call	makebool@plt
	call	print@plt
	#APP
	li	a7, 93	#exit system call
	ecall	
	#NO_APP
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	lw	s1, 4(sp)
	lw	s2, 0(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.globl	$eq
	.p2align	1
	.type	$eq,@function
$eq:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	call	streql@plt
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	$eq, .Lfunc_end2-($eq)
	.cfi_endproc

	.globl	$neq
	.p2align	1
	.type	$neq,@function
$neq:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	call	strneql@plt
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.size	$neq, .Lfunc_end3-($neq)
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

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits










