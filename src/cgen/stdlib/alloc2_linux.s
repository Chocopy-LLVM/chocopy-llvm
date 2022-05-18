# Runtime support function alloc2 (realloc).
# Prototype address is in a0.
# Number of words to allocate is in a1.
  addi sp, sp, -60
  sw ra, 28(sp)
  sw fp, 24(sp)
  addi fp, sp, 60
  sw a0, -4(fp)
  sw s1, -8(fp)
  sw a5, -12(fp)
  sw a4, -16(fp)
  sw a3, -20(fp)
  sw a2, -24(fp)
  sw a1, -28(fp)
  li a2, 4                                 # Word size in bytes
  mul a2, a1, a2                           # Calculate number of bytes to allocate
  lw t0, -28(fp)                           # Get size of object in words
  slli a0, t0, 2
  call sbrk
  lw a1, -4(fp)
alloc2_16:                                 # Copy-loop header
  lw t1, 0(a1)                             # Load next word from src
  sw t1, 0(a0)                             # Store next word to dest
  addi a1, a1, 4                           # Increment src
  addi a0, a0, 4                           # Increment dest
  addi t0, t0, -1                          # Decrement counter
  bne t0, zero, alloc2_16                  # Loop if more words left to copy
alloc2_donw:
  lw t0, -28(fp)                           # Get size of object in words
  slli a1, t0,2
  sub a0, a0,a1
  lw s1, -8(fp)
  lw a5, -12(fp)
  lw a4, -16(fp)
  lw a3, -20(fp)
  lw a2, -24(fp)
  lw a1, -28(fp)
  lw ra, 28(sp)
  lw fp, 24(sp)
  addi sp, sp, 60
  jr ra                                    # Return to caller
alloc2_15:                                 # OOM handler
  li a0, 5                                 # Exit code for: Out of memory
  la a1, const_5                           # Load error message as str
  addi a1, a1, 16                          # Load address of attribute __str__
  j abort                                  # Abort
