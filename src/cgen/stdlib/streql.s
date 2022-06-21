# Function streql
  addi sp, sp, -8
  sw ra, 4(sp)
  sw fp, 0(sp)
  addi fp, sp, 8
  lw t0, 12(a0)
  lw t1, 12(a1)
  lw t2, 16(a0)
  lw t3, 16(a1)
  bne t0, t1, streql_no
streql_1:
  lbu t4, 0(t2)
  lbu t5, 0(t3)
  bne t4, t5, streql_no
  addi t2, t2, 1
  addi t3, t3, 1
  addi t0, t0, -1
  bgtz t0, streql_1
  li a0, 1
  j streql_end
streql_no:
  xor a0, a0, a0
streql_end:
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 8
  jr ra
