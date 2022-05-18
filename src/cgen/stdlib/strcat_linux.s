# Function strcat
  addi sp, sp, -16
  sw ra, 8(sp)
  sw fp, 4(sp)
  sw a0, 12(sp)
  sw a1, 16(sp)
  addi fp, sp, 16
  lw t0, 0(fp)
  lw t1, -4(fp)
  lbu t0, 12(t0) #
  beqz t0, strcat_4
  lw t1, 12(t1)
  beqz t1, strcat_5
  add t1, t0, t1
  sw t1, -16(fp)
  addi t1, t1, 4
  srli t1, t1, 2
  addi a1, t1, 4
  la a0, $str$prototype
  jal alloc2
  sw a0, 4(fp)
  lw t0, -16(fp)
  sw t0, 12(a0)
  addi t2, a0, 16
  lw a0, -16(fp)
  call sbrk
  lw ra, 8(sp)
  sw a0, 0(t2)
  lw t0, -4(fp)
  lbu t1, 12(t0)
  addi t0, t0, 16
  lw t0, 0(t0)
  lw t2, 0(t2)
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
  lw t0, 0(t0)
strcat_3:
  beqz t1, strcat_6
  lbu t3, 0(t0)
  sb t3, 0(t2)
  addi t1, t1, -1
  addi t0, t0, 1
  addi t2, t2, 1
  j strcat_3
strcat_4:
  lw a0, -4(fp)
  j strcat_7
strcat_5:
  lw a0, 0(fp)
  j strcat_7
strcat_6:
  lw a0, 4(fp)
strcat_7:
  lw ra, -8(fp)
  lw fp, -12(fp)
  addi sp, sp, 16
  jr ra
