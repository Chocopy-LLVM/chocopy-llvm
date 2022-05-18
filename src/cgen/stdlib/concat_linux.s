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
  lw t0, 0(a0)
  sw t0, 4(fp)
  lw t1, 0(a1)
  sw t1, 0(fp)
  lw t2, 0(t0)
  beqz t2, concat_none
  lw t2, 0(t1)
  beqz t2, concat_none
  lw t0, 12(t0)
  lw t1, 12(t1)
  add s6, t0, t1
  li a1,6
  la a0, $.list$prototype
  jal alloc2
  sw s6, 12(a0)
  mv s5, a0
  addi s3, s5, 16
  sw a0, -32(fp)
  slli a0, s6, 2
  sw ra, -36(fp)
  call sbrk
  lw ra, -36(fp)
  mv s3, a0
  lw s1, 4(fp)
  lw s2, 12(s1)
  addi s1, s1, 16
  lw s1, 0(s1)
concat_1:
  beqz s2, concat_2
  lw a0, 0(s1)
  sw a0, 0(s3)
  addi s2, s2, -1
  addi s1, s1, 4
  addi s3, s3, 4
  j concat_1
concat_2:
  lw s1, 0(fp)
  lw s2, 12(s1)
  addi s1, s1, 16
  lw s1, 0(s1)
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
  lw s1, 12(a0)
  slli s1,s1,2
  sub s3,s3,s1
  sw s3,16(a0)
  lw s1, -12(fp)
  lw s2, -16(fp)
  lw s3, -20(fp)
  lw s4, -24(fp)
  lw s5, -28(fp)
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 40
  sw a0,20(a0)
  addi a0, a0, 20
  jr ra
concat_none:
  j error.None
