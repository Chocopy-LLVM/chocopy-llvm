# Runtime support function heap.init.
  addi    sp, sp, -16
  sw      ra, 12(sp)
  call sbrk                                # Code for ecall: sbrk
  lw      ra, 12(sp)                       # 4-byte Folded Reload
  addi    sp, sp, 16
  jr ra                                    # Return to caller
