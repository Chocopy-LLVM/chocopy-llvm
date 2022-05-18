# Runtime support function alloc.
        # Prototype address is in a0.
  lw a1, 4(a0)                             # Get size of object in words
  j alloc2                                 # Allocate object with exact size
