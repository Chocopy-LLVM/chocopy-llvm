# Runtime support function accstart
  mv fp, sp
  add t1, zero, zero
  lui t1, 1044480
  add sp, sp, t1
  add sp, sp, -12
  sw  a0, 0(sp)
  sw  a1, 4(sp)
  sw  a2, 8(sp)
  sw  a3, 12(sp)
  mv a3, a7
  addi a2, zero, 4
accstart_1:
  addi a2, a2, -1
  beqz a2, accstart_2
  addi a7, zero, 120
  addi a0, zero, 20
  mv a1, sp
  ecall
  bnez a0, accstart_1
accstart_2:
  mv tp, a2
  mv a7, a3
  lw  a0, 0(sp)
  lw  a1, 4(sp)
  lw  a2, 8(sp)
  lw  a3, 12(sp)
  add sp, sp, 12
  add s1, zero, zero
  lui s1, 4096
  add sp, sp, s1
  ret
