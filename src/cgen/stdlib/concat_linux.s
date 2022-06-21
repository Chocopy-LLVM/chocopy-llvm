# Function concat
  addi sp, sp, -40
  sw ra, 36(sp)
  sw fp, 32(sp)
  addi fp, sp, 40
  sw s1, -12(fp)
  sw s2, -16(fp)
  sw s3, -20(fp)
  sw s4, -24(fp)
  sw s5, -28(fp)
  beqz a0, concat_none
  beqz a1, concat_none
  lw t0, 12(a0)
  lw t1, 12(a1)
  mv t4, a0
  mv t5, a1
  add s6, t0, t1
  add a1, s6, 5
  la a0, $.list$prototype
  jal alloc2
  sw s6, 12(a0)
  mv s5, a0
  addi s3, s5, 20
  sw s3, 16(s5)
  lw s1, 16(t4)
  lw s2, 12(t4)
concat_1:
  beqz s2, concat_2
  lw a0, 0(s1)
  sw a0, 0(s3)
  addi s2, s2, -1
  addi s1, s1, 4
  addi s3, s3, 4
  j concat_1
concat_2:
  lw s1, 16(t5)
  lw s2, 12(t5)
concat_3:
  beqz s2, concat_4
  lw a0, 0(s1)
  sw a0, 0(s3)
  addi s2, s2, -1
  addi s1, s1, 4
  addi s3, s3, 4
  j concat_3
concat_4:
  mv a0, s5
  lw s1, -12(fp)
  lw s2, -16(fp)
  lw s3, -20(fp)
  lw s4, -24(fp)
  lw s5, -28(fp)
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 40
  jr ra
concat_none:
  j error.None
