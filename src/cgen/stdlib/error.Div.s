  li a0, 2                                 # Exit code for: Division by zero
  la a1, const_7                           # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
