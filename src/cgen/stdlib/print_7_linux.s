# Printing ints.
# Print int object in A0
  lw a1, 12(a0)                            # Load attribute __int__
  lui a0, %hi(char_int)                    # Load high bits for "%i"
  addi a0, a0, %lo(char_int)
  addi sp,sp,-16
  sw ra, 0(sp)
  call myprintf                            # Call printf
  lw ra, 0(sp)
  addi sp,sp,16
  jr ra
char_int:
  .asciz  "%d\\n"
