  li a0, 3                                 # Exit code for: Index out of bounds
  la a1, const_8                           # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
