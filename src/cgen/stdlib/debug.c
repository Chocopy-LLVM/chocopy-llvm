#include "debug.h"
char text_[] = "err!";
volatile int wait_ = 1;
int debug_() {
    while (wait_)
        ;
    /** Doesn't actually go on the stack, because there are lots of GPRs. */
    int i = 0;
    while (text_[i]) {
        char lower = text_[i] | 32;
        if (lower >= 'a' && lower <= 'm')
            text_[i] += 13;
        else if (lower > 'm' && lower <= 'z')
            text_[i] -= 13;
        i++;
    }
    while (!wait_)
        ;
}