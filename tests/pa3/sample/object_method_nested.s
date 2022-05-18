	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"object_method_nested.py"
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
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	#APP
	mv	s0, sp
	#NO_APP
	lui	a0, %hi(($B$prototype))
	addi	s0, a0, %lo(($B$prototype))
	mv	a0, s0
	call	alloc@plt
	lw	a1, 8(a0)
	lw	a1, 0(a1)
	jalr	a1
	mv	a0, s0
	call	alloc@plt
	lw	a1, 8(a0)
	lw	a1, 8(a1)
	jalr	a1
	call	makeint@plt
	call	print@plt
	#APP
	li	a7, 93	#exit system call
	ecall	
	#NO_APP
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.globl	$A.foo
	.p2align	1
	.type	$A.foo,@function
$A.foo:
	lw	a0, 12(a0)
	ret
.Lfunc_end2:
	.size	$A.foo, .Lfunc_end2-($A.foo)

	.globl	$B.__init__
	.p2align	1
	.type	$B.__init__,@function
$B.__init__:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	lui	a0, %hi(const_9)
	addi	a0, a0, %lo(const_9)
	call	print@plt
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.size	$B.__init__, .Lfunc_end3-($B.__init__)
	.cfi_endproc

	.globl	$B.bar
	.p2align	1
	.type	$B.bar,@function
$B.bar:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	lui	a0, %hi(($B$prototype))
	addi	a0, a0, %lo(($B$prototype))
	call	alloc@plt
	sw	a0, 0(sp)
	sw	a0, 4(sp)
	lw	a1, 8(a0)
	lw	a2, 4(a1)
	mv	a1, sp
	jalr	a2
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end4:
	.size	$B.bar, .Lfunc_end4-($B.bar)
	.cfi_endproc

	.globl	$bar.qux
	.p2align	1
	.type	$bar.qux,@function
$bar.qux:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	mv	a1, a0
	lw	a0, 0(a0)
	lw	a2, 8(a0)
	lw	a2, 4(a2)
	jalr	a2
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end5:
	.size	$bar.qux, .Lfunc_end5-($bar.qux)
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

	.type	$A$prototype,@object
	.globl	$A$prototype
	.p2align	3
$A$prototype:
	.word	7
	.word	4
	.word	($A$dispatchTable)
	.word	42
	.size	$A$prototype, 16

	.type	$A$dispatchTable,@object
	.section	.sdata,"aw",@progbits
	.globl	$A$dispatchTable
	.p2align	3
$A$dispatchTable:
	.word	($object.__init__)
	.word	($A.foo)
	.size	$A$dispatchTable, 8

	.type	$B$prototype,@object
	.data
	.globl	$B$prototype
	.p2align	4
$B$prototype:
	.word	8
	.word	5
	.word	($B$dispatchTable)
	.word	42
	.byte	1
	.zero	3
	.size	$B$prototype, 20

	.type	$B$dispatchTable,@object
	.globl	$B$dispatchTable
	.p2align	3
$B$dispatchTable:
	.word	($B.__init__)
	.word	($A.foo)
	.word	($B.bar)
	.size	$B$dispatchTable, 12

	.type	const_9,@object
	.globl	const_9
	.p2align	4
const_9:
	.word	3
	.word	5
	.word	($str$dispatchTable)
	.word	1
	.word	.Lstr.const_9
	.size	const_9, 20

	.type	.Lstr.const_9,@object
	.section	.sdata,"aw",@progbits
.Lstr.const_9:
	.asciz	"B"
	.size	.Lstr.const_9, 2

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits














