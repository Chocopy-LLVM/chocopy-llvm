# Runtime support function abort (does not return).
  mv t0, a0                                # Save exit code in temp
  lui a0, %hi(error_char)                  # Load high bit of error_char
  addi a0, a0, %lo(error_char)             # Load low bit of error_char
  call myprintf                            # Print error message
  mv a1, t0                                # Move exit code to a1
  li a7, 93                                # Code for exit2 ecall
  ecall                                    # Exit with code
abort_17:                                  # Infinite loop
  j abort_17                               # Prevent fallthrough

error_char:
  .asciz  "%s\\n"
