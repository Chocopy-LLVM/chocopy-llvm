# Print null-terminated string in A1
  lui a0, %hi(char_string)                 # Load high bits for "%s"
  addi a0, a0, %lo(char_string)            # Load low bits for "%s"
  addi sp, sp, -16
  sw ra, 12(sp)
  call myprintf                            # Call printf
  lw ra, 12(sp)
  addi sp, sp, 16
  j print_5                                # Go to return
char_string:
  .asciz  "%s\\n"
