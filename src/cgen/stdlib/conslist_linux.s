  # Function conslist:
  addi sp, sp, -60
  sw ra, 28(sp)
  sw fp, 24(sp)
  addi fp, sp, 60
  sw gp, -4(fp)
  sw s1, -8(fp)
  sw a5, -12(fp)
  sw a4, -16(fp)
  sw a3, -20(fp)
  sw a2, -24(fp)
  sw a1, -28(fp)
  add a1,zero, a0
  sw a0, -40(fp)
  beqz a1, conslist_done_1
  # a1 pass for malloc the array
  slli a0, a1, 2
  call sbrk
  lw t0, -40(fp)
  slli t3, t0, 2
  add t2, a0, t3
  addi t1, fp, -28
  add t1, t1, t3
conslist_1:
  lw t3, -4(t1)
  sw t3, -4(t2)
  addi t1, t1, -4
  addi t2, t2, -4
  addi t0, t0, -1
  bnez t0, conslist_1
conslist_2:
  # get the address of array
  sw t2, -44(fp)
  la a0, $.list$prototype
  li a1, 6
  jal alloc2
  lw t3, -40(fp)
  lw t2, -44(fp)
  sw t3, 12(a0)
  sw t2, 16(a0)
conslist_done:
  sw a0, 20(a0)
  add a0, a0, 20
  lw gp, -4(fp)
  lw s1, -8(fp)
  lw a5, -12(fp)
  lw a4, -16(fp)
  lw a3, -20(fp)
  lw a2, -24(fp)
  lw a1, -28(fp)
  lw ra, 28(sp)
  lw fp, 24(sp)
  addi sp, sp, 60
  jr ra
conslist_done_1:
  la a0, $.list$prototype
  li a1, 6
  jal alloc2
  j conslist_done
