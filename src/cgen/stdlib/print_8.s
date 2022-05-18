# Printing strs.
# Print str object in A0
  addi a1, a0, 16                          # Load address of attribute __str__
  j print_11                               # Print the null-terminated string is now in A1
  mv a0, zero                              # Load None
  j print_5                                # Go to return
