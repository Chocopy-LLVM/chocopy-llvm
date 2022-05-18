# Function input
  addi sp, sp, -16                         # Reserve stack	
  sw ra, 12(sp)                            # Save registers
  sw fp, 8(sp)	
  sw s1, 4(sp)
  addi fp, sp, 16                          # Set fp

  li a0, 18                                # Fill the internal line buffer.
  ecall
  bgez a0, input_nonempty                  # More input found
  la a0, $str$prototype                    # EOF: Return empty string.
  j input_done

input_nonempty:
  mv s1, a0
  addi t0, s1, 5                           # Compute bytes for string (+NL+NUL),
  addi t0, t0, 16                          # Including header.
  srli a1, t0, 2                           # Convert to words.
  la a0, $str$prototype                    # Load address of string prototype.
  jal ra, alloc2                           # Allocate string.
  sw s1, 12(a0)                            # Store string length.
  mv a2, s1                                # Pass length.
  mv s1, a0                                # Save string object address.
  addi a1, a0, 16                          # Pass address of string data.
  li a0, 8                                 # ecall to read from internal buffer.
  ecall
  addi a0, a0, 1                           # Actual length (including NL).
  sw a0, 12(s1)                            # Store actual length.
  add t0, a0, s1
  li t1, 10                                # Store newline and null byte
  sb t1, 15(t0)
  sb zero, 16(t0)                          # Store null byte at end.
  mv a0, s1                                # Return string object.

input_done:
  lw s1, -12(fp)
  lw ra, -4(fp)
  lw fp, -8(fp)
  addi sp, sp, 16
  jr ra