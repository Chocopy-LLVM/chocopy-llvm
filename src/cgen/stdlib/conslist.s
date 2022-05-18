# Function conslist:
  addi sp, sp, -8
  sw ra, 4(sp)
  sw fp, 0(sp)
  addi fp, sp, 8
  lw a1, 0(fp)
  la a0, $.list$prototype
  beqz a1, conslist_done
  addi a1, a1, 4
  jal alloc2
  lw t0, 0(fp)
  sw t0, 12(a0)
  slli t1, t0, 2
  add t1, t1, fp
  addi t2, a0, 16
conslist_1:
  lw t3, 0(t1)
  sw t3, 0(t2)
  addi t1, t1, -4
  addi t2, t2, 4
  addi t0, t0, -1
  bnez t0, conslist_1
conslist_done:
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 8
  jr ra
