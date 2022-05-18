# Runtime support function accend
  beqz s1, .accend_2
.accend_1:
  call exit
.accend_2:
  addi sp, sp, -16
  sw  a0, 0(sp)
  sw  a1, 4(sp)
  sw  a2, 8(sp)
  sw  a3, 12(sp)
  addi a1, zero, 4
.accend_3:
  addi a1, a1, -1
  beqz a1, .accend_4
  addi sp, sp, -8
  sw  a1, 0(sp)
  sw  ra, 4(sp)
  addi sp, sp, -4
  mv a0, sp
  jal wait
  addi sp, sp, 4
  lw  a1, 0(sp)
  lw  ra, 4(sp)
  addi sp, sp, 8
  j .accend_3
.accend_4:
  lw  a0, 0(sp)
  lw  a1, 4(sp)
  lw  a2, 8(sp)
  lw  a3, 12(sp)
  addi sp, sp, 16
  mv s1, zero
  ret
