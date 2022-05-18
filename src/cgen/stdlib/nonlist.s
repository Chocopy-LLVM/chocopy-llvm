  addi sp, sp, -32
  sw ra, 28(sp)
  sw fp, 24(sp)
  addi fp, sp,32
  li a0, 6
  call sbrk
  addi a0,a0,4
  sw a0, -4(a0)
  addi a0,a0,-4
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 32
  jr ra
