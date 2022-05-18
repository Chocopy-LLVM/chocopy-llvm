 addi sp, sp, -8
 sw ra, 4(sp)
 sw a0, 0(sp)
 la a0, $int$prototype
 jal ra, alloc
 lw t0, 0(sp)
 sw t0, 12(a0)
 lw ra, 4(sp)
 addi sp, sp, 8
 jr ra
 