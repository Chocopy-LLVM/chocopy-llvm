# Function strneql
  addi sp, sp, -8
  sw ra, 4(sp)
  sw fp, 0(sp)
  addi fp, sp, 8
  lw t0, 12(a0)
  lw t1, 12(a1)
  lw a0, 16(a0)
  lw a1, 16(a1)
  bne t0, t1, strneql_yes
strneql_1:
  lbu t2, 0(a0)
  lbu t3, 0(a1)
  bne t2, t3, strneql_yes
  addi a0, a0, 1
  addi a1, a1, 1
  addi t0, t0, -1
  bgtz t0, strneql_1
  xor a0, a0, a0
  j strneql_end
strneql_yes:
  li a0, 1
strneql_end:
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 8
  jr ra
