# Function len
      # We do not save/restore fp/ra for this function
      # because we know that it does not use the stack or does not
      # call other functions.

  lw a0, 0(sp)                             # Load arg
  beq a0, zero, len_12                     # None is an illegal argument
  lw t0, 0(a0)                             # Get type tag of arg
  li t1, 3                                 # Load type tag of `str`
  beq t0, t1, len_13                       # Go to len(str)
  li t1, -1                                # Load type tag for list objects
  beq t0, t1, len_13                       # Go to len(list)
len_12:                                    # Invalid argument
  li a0, @error_arg                        # Exit code for: Invalid argument
  la a1, STRING["Invalid argument"]        # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
len_13:                                    # Get length of string
  lw a0, 12(a0)                            # Load attribute: __len__
  jr ra                                    # Return to caller
