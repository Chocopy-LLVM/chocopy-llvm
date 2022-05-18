	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"stmt_for_list_return.py"
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
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sw	ra, 28(sp)
	sw	s0, 24(sp)
	sw	s1, 20(sp)
	sw	s2, 16(sp)
	sw	s3, 12(sp)
	sw	s4, 8(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	.cfi_offset s2, -16
	.cfi_offset s3, -20
	.cfi_offset s4, -24
	#APP
	mv	s0, sp
	#NO_APP
	li	a0, 4
	li	a1, 10
	li	a2, 20
	li	a3, 30
	li	a4, 40
	call	conslist@plt
	mv	s2, a0
	call	($len)@plt
	li	s0, 0
	addi	s1, a0, -1
.LBB1_1:
	lw	a0, 0(s2)
	lw	a0, 16(a0)
	add	a0, a0, s0
	lw	s3, 0(a0)
	seqz	s4, s1
	mv	a0, s3
	call	makeint@plt
	call	print@plt
	slti	a0, s3, 30
	xori	a0, a0, 1
	or	a0, a0, s4
	addi	s0, s0, 4
	addi	s1, s1, -1
	beqz	a0, .LBB1_1
	#APP
	li	a7, 93	#exit system call
	ecall	
	#NO_APP
	lw	ra, 28(sp)
	lw	s0, 24(sp)
	lw	s1, 20(sp)
	lw	s2, 16(sp)
	lw	s3, 12(sp)
	lw	s4, 8(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.globl	$print_list
	.p2align	1
	.type	$print_list,@function
$print_list:
	.cfi_startproc
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sw	ra, 28(sp)
	sw	s0, 24(sp)
	sw	s1, 20(sp)
	sw	s2, 16(sp)
	sw	s3, 12(sp)
	sw	s4, 8(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	.cfi_offset s1, -12
	.cfi_offset s2, -16
	.cfi_offset s3, -20
	.cfi_offset s4, -24
	mv	s2, a0
	call	($len)@plt
	li	s0, 0
	addi	s1, a0, -1
.LBB2_1:
	lw	a0, 0(s2)
	lw	a0, 16(a0)
	add	a0, a0, s0
	lw	s3, 0(a0)
	seqz	s4, s1
	mv	a0, s3
	call	makeint@plt
	call	print@plt
	slti	a0, s3, 30
	xori	a0, a0, 1
	or	a0, a0, s4
	addi	s0, s0, 4
	addi	s1, s1, -1
	beqz	a0, .LBB2_1
	lw	ra, 28(sp)
	lw	s0, 24(sp)
	lw	s1, 20(sp)
	lw	s2, 16(sp)
	lw	s3, 12(sp)
	lw	s4, 8(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end2:
	.size	$print_list, .Lfunc_end2-($print_list)
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

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits







