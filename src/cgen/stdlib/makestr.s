# Init for multiple length string literal
 addi sp, sp, -8
 sw ra, 8(sp)
 sw a0, 0(sp)
 sw a1, 4(sp)
 la a0, $str$prototype
 jal ra, alloc
 lw t0, 0(sp)
 lw t1, 4(sp)
 sw t0, 16(a0)
 sw t0, 12(a0)
 lw ra, 8(sp)
 addi sp, sp, 8
 jr ra

