# Printing ints.
# Print int object in A0
  lw a1, 12(a0)                            # Load attribute __int__
  li a0, 1                                 # Code for ecall: print_int
  ecall                                    # Print integer
  li a1, 10                                # Load newline character
  li a0, 11                                # Code for ecall: print_char
  ecall                                    # Print character
