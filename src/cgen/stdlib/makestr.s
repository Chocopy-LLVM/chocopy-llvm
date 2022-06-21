# Init for multiple length string literal
 addi sp, sp, -8
 sw ra, 4(sp)
 sw a0, 0(sp)
 la a0, $str$prototype
 li a1, 6
 jal alloc2
 lw t0, 0(sp)
 li t1, 1
 sw t1, 12(a0)
 add t1, a0, 20
 sw t1, 16(a0)
 sb t0, 20(a0)
 lw ra, 4(sp)
 addi sp, sp, 8
 jr ra

