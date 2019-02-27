#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct bfp {
    char *source; // brainfuck source text, padded with \0 at each end. First instruction at index 1
    size_t length; // total source text length, included \0-padding
} bf_program;

bf_program *parse_file(FILE *file, int64_t size);

void run_bf(bf_program *program);

uint8_t *expand_tape(uint8_t *tape, size_t tape_len);


#endif //BRAINFUCK_H
