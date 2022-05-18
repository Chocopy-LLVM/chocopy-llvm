# Function streql
  addi sp, sp, -8
  sw ra, 4(sp)
  sw fp, 0(sp)
  addi fp, sp, 8
  lw a1, 4(fp)
  lw a2, 0(fp)
  lw t0, 12(a1)
  lw t1, 12(a2)
  bne t0, t1, streql_no
streql_1:
  lbu t2, 16(a1)
  lbu t3, 16(a2)
  bne t2, t3, streql_no
  addi a1, a1, 1
  addi a2, a2, 1
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
