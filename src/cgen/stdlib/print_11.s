# Print null-terminated string in A1
  li a0, 4                                 # Code for ecall: print_string
  ecall                                    # Print string
  li a1, 10                                # Load newline character
  li a0, 11                                # Code for ecall: print_char
  ecall                                    # Print character
  j print_5                                # Go to return
