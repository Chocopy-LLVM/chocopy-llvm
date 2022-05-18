/** For debugging spike https://github.com/riscv-software-src/riscv-isa-sim */
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char text_[];

// Don't use the stack, because sp isn't set up.
extern volatile int wait_;

int debug();