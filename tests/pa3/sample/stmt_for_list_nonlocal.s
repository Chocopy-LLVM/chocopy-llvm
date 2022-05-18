	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"stmt_for_list_nonlocal.py"
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
	li	a0, 2
	li	a1, 1
	li	a2, 2
	call	conslist@plt
	lw	s2, 0(a0)
	li	a0, 2
	li	a1, 2
	li	a2, 3
	call	conslist@plt
	lw	s1, 0(a0)
	li	a0, 2
	li	a1, 4
	li	a2, 5
	call	conslist@plt
	lw	s0, 0(a0)
	li	a0, 2
	li	a1, 6
	li	a2, 7
	call	conslist@plt
	lw	a4, 0(a0)
	li	a0, 4
	mv	a1, s2
	mv	a2, s1
	mv	a3, s0
	call	conslist@plt
	mv	s0, a0
	call	($len)@plt
	lw	a1, 0(s0)
	lw	a1, 16(a1)
	slli	a0, a0, 2
	add	a0, a0, a1
	addi	s0, a0, -4
	mv	a0, s0
	call	($len)@plt
	li	a2, 0
	lui	a3, %hi(x)
.LBB1_1:
	lw	a1, 0(s0)
	lw	a1, 16(a1)
	add	a1, a1, a2
	lw	a1, 0(a1)
	sw	a1, %lo(x)(a3)
	addi	a0, a0, -1
	addi	a2, a2, 4
	bnez	a0, .LBB1_1
	mv	a0, a1
	call	makeint@plt
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

	.globl	$crunch
	.p2align	1
	.type	$crunch,@function
$crunch:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	sw	s0, 8(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	mv	s0, a0
	call	($len)@plt
	lw	a1, 0(s0)
	lw	a1, 16(a1)
	slli	a0, a0, 2
	add	a0, a0, a1
	addi	s0, a0, -4
	mv	a0, s0
	call	($len)@plt
	li	a1, 0
	lui	a2, %hi(x)
.LBB2_1:
	lw	a3, 0(s0)
	lw	a3, 16(a3)
	add	a3, a3, a1
	lw	a3, 0(a3)
	sw	a3, %lo(x)(a2)
	addi	a0, a0, -1
	addi	a1, a1, 4
	bnez	a0, .LBB2_1
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	$crunch, .Lfunc_end2-($crunch)
	.cfi_endproc

	.globl	$crunch.make_z
	.p2align	1
	.type	$crunch.make_z,@function
$crunch.make_z:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	sw	s0, 8(sp)
	sw	s1, 4(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	mv	s0, a0
	lw	s1, 4(a0)
	mv	a0, s1
	call	($len)@plt
	li	a1, 0
.LBB3_1:
	lw	a2, 0(s1)
	lw	a2, 16(a2)
	add	a2, a2, a1
	sw	a2, 0(s0)
	addi	a0, a0, -1
	addi	a1, a1, 4
	bnez	a0, .LBB3_1
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	lw	s1, 4(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.size	$crunch.make_z, .Lfunc_end3-($crunch.make_z)
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

	.type	x,@object
	.section	.sbss,"aw",@nobits
	.globl	x
	.p2align	2
x:
	.word	0
	.size	x, 4

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits







