#include "debug.h"

#define INITIAL_BUFFER_SIZE 2 /* for testing */

struct str_dispatch {
    int id;
    int size;
    int $str$prototype;
    int size_of_str;
    char *str;
};

struct str_dispatch *myscanf(struct str_dispatch *old) {
    int current_buffer_size = INITIAL_BUFFER_SIZE;
    char *line = malloc(current_buffer_size);
    strcpy(text_, "Jump");
    if (line == NULL) {
        return old;
    }
    //    FILE *input = fopen("./input.py.in", "r");

    setvbuf(stdin, (char *)NULL, _IOFBF, 0);

    int offset = 0;
    while (fgets(line + offset, 2, stdin)) {
        if (line[strlen(line) - 1] == '\n' && strlen(line) != 1) {
            line[strlen(line) - 1] = '\0';
            break;
        } else if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1];
        } else {
            int new_buffer_size = 1 + current_buffer_size;
            char *tmp = realloc(line, new_buffer_size);
            if (tmp) {
                line = tmp;
                offset = current_buffer_size - 1;
                current_buffer_size = new_buffer_size;
            } else {
                break;
            }
        }
    }

    old->size_of_str = strlen(line);
    old->str = line;
    return old;
}