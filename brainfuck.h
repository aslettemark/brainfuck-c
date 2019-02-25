#ifndef BRAINFUCK_C_BRAINFUCK_H
#define BRAINFUCK_C_BRAINFUCK_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct bfp {
    char *source;
    size_t length;
} bf_program;

bf_program *parse_file(FILE *file, int64_t size);

#endif //BRAINFUCK_C_BRAINFUCK_H
