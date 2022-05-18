# Runtime support function alloc2 (realloc).
# Prototype address is in a0.
# Number of words to allocate is in a1.
  li a2, 4                                 # Word size in bytes
  mul a2, a1, a2                           # Calculate number of bytes to allocate
  add a2, gp, a2                           # Estimate where GP will move
  bgeu a2, s11, alloc2_15                  # Go to OOM handler if too large
  lw t0, 4(a0)                             # Get size of object in words
  mv t2, a0                                # Initialize src ptr
  mv t3, gp                                # Initialize dest ptr
alloc2_16:                                 # Copy-loop header
  lw t1, 0(t2)                             # Load next word from src
  sw t1, 0(t3)                             # Store next word to dest
  addi t2, t2, 4                           # Increment src
  addi t3, t3, 4                           # Increment dest
  addi t0, t0, -1                          # Decrement counter
  bne t0, zero, alloc2_16                  # Loop if more words left to copy
  mv a0, gp                                # Save new object's address to return
  sw a1, 4(a0)                             # Set size of new object in words
  mv gp, a2                                # Set next free slot in the heap
  jr ra                                    # Return to caller
alloc2_15:                                 # OOM handler
  li a0, 5                                 # Exit code for: Out of memory
  la a1, const_5                           # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
