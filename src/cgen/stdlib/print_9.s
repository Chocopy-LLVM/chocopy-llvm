# Printing bools
# Print bool object in A0
  lw a0, 12(a0)                            # Load attribute __bool__
  beq a0, zero, print_10                   # Go to: print(False)
  la a0, const_3                           # String representation: True
  j print_8                                # Go to: print(str)
