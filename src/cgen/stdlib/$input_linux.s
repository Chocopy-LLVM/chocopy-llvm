# Function input
  addi sp, sp, -16                         # Reserve stack
  sw ra, 12(sp)                            # Save registers
  sw fp, 8(sp)
  sw s1, 4(sp)
  addi fp, sp, 16                          # Set fp
  la a0, $str$prototype                    # EOF: Return empty string.
  call myscanf
  lw t1, 12(a0)                            # Restore registers
  bne t1, zero, input_done                      # Go to the end
  la a0, $str$prototype                    # EOF: Return empty string.
  j input_done
input_done:
  lw s1, -12(fp)
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 16
  jr ra
