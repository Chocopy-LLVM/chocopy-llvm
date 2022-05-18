/** https://scc-forge.lancaster.ac.uk/open/char/io/printfv */

/**
    putchar is the only external dependency for this file,
    if you have a working putchar, just remove the following
    define. If the function should be called something else,
    replace outbyte(c) by your own function call.
*/
#include "debug.h"

extern write(int, char *, int);

static void printchar(char **str, char c) {
    if (str) {
        **str = c;
        ++(*str);
    } else
        (void)write(1, &c, 1);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad) {
    int pc = 0, padchar = ' ';

    if (width > 0) {
        int len = 0;
        const char *ptr;
        for (ptr = string; *ptr; ++ptr)
            ++len;
        if (len >= width)
            width = 0;
        else
            width -= len;
        if (pad & PAD_ZERO)
            padchar = '0';
    }
    if (!(pad & PAD_RIGHT)) {
        for (; width > 0; --width) {
            printchar(out, padchar);
            ++pc;
        }
    }
    for (; *string; ++string) {
        printchar(out, *string);
        ++pc;
    }
    for (; width > 0; --width) {
        printchar(out, padchar);
        ++pc;
    }

    return pc;
}

/** the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase) {
    char print_buf[PRINT_BUF_LEN];
    char *s;
    int t, neg = 0, pc = 0;
    unsigned int u = i;

    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints(out, print_buf, width, pad);
    }

    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN - 1;
    *s = '\0';

    while (u) {
        t = u % b;
        if (t >= 10)
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg) {
        if (width && (pad & PAD_ZERO)) {
            printchar(out, '-');
            ++pc;
            --width;
        } else {
            *--s = '-';
        }
    }

    return pc + prints(out, s, width, pad);
}

int myprintf(char *format, int varg) {
    int width, pad;
    int pc = 0;
    char scr[2];

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = pad = 0;
            if (*format == '\0')
                break;
            if (*format == '%')
                goto out;
            if (*format == '-') {
                ++format;
                pad = PAD_RIGHT;
            }
            while (*format == '0') {
                ++format;
                pad |= PAD_ZERO;
            }
            for (; *format >= '0' && *format <= '9'; ++format) {
                width *= 10;
                width += *format - '0';
            }
            if (*format == 's') {
                /** check the first letter is char or the type is char* */
                bool is_str = false;
                for (int i = 0; i < 3; i++) {
                    if(i==0){
                        is_str=isascii(*((char *)varg + i));
                    }
                    is_str &= isalpha(*((char *)varg + i)) || isupper(*((char *)varg + i));
                }
                if (is_str)
                    pc += prints(0, varg, width, pad);
                else if (*(char **)varg) {
                    pc += prints(0, *(char **)varg, width, pad);
                } else
                    pc += prints(0, "", width, pad);

                continue;
            }
            if (*format == 'd') {
                pc += printi(0, varg, 10, 1, width, pad, 'a');
                continue;
            }
            if (*format == 'x') {
                pc += printi(0, varg, 16, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'X') {
                pc += printi(0, varg, 16, 0, width, pad, 'A');
                continue;
            }
            if (*format == 'u') {
                pc += printi(0, varg, 10, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'c') {
                /** char are converted to int then pushed on the stack */
                scr[0] = varg++;
                scr[1] = '\0';
                pc += prints(0, scr, width, pad);
                continue;
            }
        } else {
        out:
            printchar(0, *format);
            ++pc;
        }
    }
    return pc;
}