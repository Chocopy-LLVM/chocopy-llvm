  li a0, 4                                 # Exit code for: Operation on None
  la a1, const_6                           # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
