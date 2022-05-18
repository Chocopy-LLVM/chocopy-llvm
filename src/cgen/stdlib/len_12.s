# Invalid argument
  li a0, 1                                 # Exit code for: Invalid argument
  la a1, const_2                           # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
