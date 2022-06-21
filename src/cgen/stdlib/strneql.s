# Function strneql
  addi sp, sp, -8
  sw ra, 4(sp)
  sw fp, 0(sp)
  addi fp, sp, 8
  lw t0, 12(a0)
  lw t1, 12(a1)
  lw t2, 16(a0)
  lw t3, 16(a1)
  bne t0, t1, strneql_no
strneql_1:
  lbu t4, 0(t2)
  lbu t5, 0(t3)
  bne t4, t5, strneql_no
  addi t2, t2, 1
  addi t3, t3, 1
  addi t0, t0, -1
  bgtz t0, strneql_1
  xor a0, a0, a0
  j strneql_end
strneql_no:
  li a0, 1
strneql_end:
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 8
  jr ra
