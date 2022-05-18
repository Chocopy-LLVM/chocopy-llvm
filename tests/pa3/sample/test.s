	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.file	"test.ll"
	.globl	main
	.p2align	1
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sw	ra, 28(sp)
	sw	s0, 24(sp)
	.cfi_offset ra, -4
	.cfi_offset s0, -8
	addi	s0, sp, 32
	.cfi_def_cfa s0, 0
	li	a0, 0
	sw	a0, -28(s0)
	sw	a0, -12(s0)
	sw	a0, -16(s0)
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	call	printf
	addi	a0, s0, -16
	sw	a0, -24(s0)
	addi	a0, s0, -24
	call	_ZZ4mainENK3$_0clEv
	lw	a1, -16(s0)
	lui	a0, %hi(.L.str.1)
	addi	a0, a0, %lo(.L.str.1)
	call	printf
	lui	a0, %hi(.L.str.2)
	addi	a0, a0, %lo(.L.str.2)
	call	printf
	lw	a0, -28(s0)
	lw	ra, 28(sp)
	lw	s0, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.p2align	1
	.type	_ZZ4mainENK3$_0clEv,@function
_ZZ4mainENK3$_0clEv:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	sw	a0, 4(sp)
	sw	a0, 8(sp)
	lw	a1, 0(a0)
	lw	a0, 0(a1)
	addi	a0, a0, 1
	sw	a0, 0(a1)
	lui	a0, %hi(.L.str.3)
	addi	a0, a0, %lo(.L.str.3)
	call	printf
	lw	a0, 4(sp)
	lw	a0, 0(a0)
	lw	a0, 0(a0)
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	_ZZ4mainENK3$_0clEv, .Lfunc_end1-_ZZ4mainENK3$_0clEv
	.cfi_endproc

	.type	.L.str,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Main\n"
	.size	.L.str, 6

	.type	.L.str.1,@object
.L.str.1:
	.asciz	"%d"
	.size	.L.str.1, 3

	.type	.L.str.2,@object
.L.str.2:
	.asciz	"GEEKS"
	.size	.L.str.2, 6

	.type	.L.str.3,@object
.L.str.3:
	.asciz	"View\n"
	.size	.L.str.3, 6

	.section	".note.GNU-stack","",@progbits
