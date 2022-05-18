# Function len
# We do not save/restore fp/ra for this function
# because we know that it does not use the stack or does not
# call other functions.
  beq a0, zero, len_12                     # None is an illegal argument
  lw t0, 0(a0)                             # Get type tag of arg
  li t1, 3                                 # Load type tag of `str`
  beq t0, t1, len_13                       # Go to len(str)
  li t1, -1                                # Load type tag for list objects
  beq t0, t1, len_13                       # Go to len(list)
  li t1, 1
  beq t0, t1, len_12
  li t1, 2
  beq t0, t1, len_12
  lw t0, 0(t0)                             # Get length of list
  lw a0, 0(a0)                             # Get list
  li t1, 3                                 # Load type tag of `str`
  beq t0, t1, len_13                       # Go to len(str)
  li t1, -1                                # Load type tag for list objects
  ble t0, t1, len_13                       # Go to len(list)
  j len_12                                 # Go to len(None)
len_14:
  li a0, 0
  ja ra