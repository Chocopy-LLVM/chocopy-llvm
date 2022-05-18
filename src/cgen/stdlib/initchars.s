# Only init for one variable
 sw ra, 16(sp)
 sw fp, 12(sp)
 sw a0, 4(sp)
 la a0, $str$prototype
 li a1,5
 jal alloc2
 li t3, 1
 sw t3, 12(a0)
 sw a0, 8(sp)
 li a0, 4
 call sbrk
 lw t4, 4(sp)
 sw t4, 0(a0)
 lw t3, 8(sp)
 sw a0, 16(t3)
 mv a0, t3
 lw ra, 16(sp)
 lw fp, 12(sp)
 jr  ra
