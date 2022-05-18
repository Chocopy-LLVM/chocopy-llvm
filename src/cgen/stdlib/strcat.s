# Function strcat
  addi sp, sp, -12
  sw ra, 8(sp)
  sw fp, 4(sp)
  sw a0, 4(fp)
  sw a1, 0(fp)
  addi fp, sp, 12
  lw t0, 4(fp)
  lw t1, 0(fp)
  lbu t0, 12(t0)
  beqz t0, strcat_4
  lw t1, 12(t1)
  beqz t1, strcat_5
  add t1, t0, t1
  sw t1, -12(fp)
  addi t1, t1, 4
  srli t1, t1, 2
  addi a1, t1, 4
  la a0, $str$prototype
  jal alloc2
  lw t0, -12(fp)
  sw t0, 12(a0)
  addi t2, a0, 16
  lw t0, 4(fp)
  lbu t1, 12(t0)
  addi t0, t0, 16
strcat_1:
  beqz t1, strcat_2
  lbu t3, 0(t0)
  sb t3, 0(t2)
  addi t1, t1, -1
  addi t0, t0, 1
  addi t2, t2, 1
  j strcat_1
strcat_2:
  lw t0, 0(fp)
  lw t1, 12(t0)
  addi t0, t0, 16
strcat_3:
  beqz t1, strcat_6
  lbu t3, 0(t0)
  sb t3, 0(t2)
  addi t1, t1, -1
  addi t0, t0, 1
  addi t2, t2, 1
  j strcat_3
strcat_4:
  lw a0, 0(fp)
  j strcat_7
strcat_5:
  lw a0, 4(fp)
  j strcat_7
strcat_6:
  sb zero, 0(t2)
strcat_7:
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 12
  jr ra
