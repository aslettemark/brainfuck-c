#include "brainfuck.h"

int main(int argc, char **argv) {
    FILE *file;
    if (argc != 2) {
        fprintf(stderr, "./brainfuck <file.bf>\n");
        exit(1);
    }
    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", argv[1]);
        exit(1);
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error reading file: %s\n", argv[1]);
        exit(1);
    }
    int64_t fsize = ftell(file);
    if (fsize == -1) {
        fprintf(stderr, "Error reading file: %s\n", argv[1]);
        exit(1);
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error reading file: %s\n", argv[1]);
        exit(1);
    }

    bf_program *program = parse_file(file, fsize);
    run_bf(program);

    return 0;
}

void run_bf(bf_program *program) {
    size_t tape_len = 8;
    uint8_t *tape = calloc(tape_len, sizeof(uint8_t));
    uint8_t *tape_ptr = tape;
    uint8_t *tape_upper_bound = tape + tape_len;

    char *instruction_ptr = program->source;
    instruction_ptr++; // First instruction at index 1

    char instruction;
    int c_in;
    int nesting;
    while ((instruction = *instruction_ptr) != '\0') {
        switch (instruction) {
            case '+':
                *tape_ptr += 1;
                break;
            case '-':
                *tape_ptr -= 1;
                break;
            case '[':
                if (*tape_ptr != 0) break;
                nesting = 1;
                while (nesting > 0) {
                    instruction_ptr++;
                    switch (*instruction_ptr) {
                        case '[':
                            nesting++;
                            break;
                        case ']':
                            nesting--;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case ']':
                if (*tape_ptr == 0) break;
                nesting = 1;
                while (nesting > 0) {
                    instruction_ptr--;
                    switch (*instruction_ptr) {
                        case '[':
                            nesting--;
                            break;
                        case ']':
                            nesting++;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case '.':
                printf("%c", *tape_ptr);
                break;
            case ',':
                c_in = getchar();
                if (c_in == EOF) {
                    exit(2);
                }
                *tape_ptr = (uint8_t) c_in;
                break;
            case '<':
                tape_ptr--;
                if (tape_ptr < tape) {
                    fprintf(stderr, "Out of tape!\n");
                    exit(1);
                }
                break;
            case '>':
                tape_ptr++;
                if (tape_ptr >= tape_upper_bound) {
                    // Double tape length
                    tape = expand_tape(tape, tape_len);
                    tape_ptr = tape + tape_len;
                    tape_len = 2 * tape_len;
                    tape_upper_bound = tape + tape_len;
                }
                break;
            default:
                fprintf(stderr, "Interpreter error\n");
                exit(1);
        }
        instruction_ptr++;
    }

    free(tape);
}

uint8_t *expand_tape(uint8_t *tape, size_t tape_len) {
    uint8_t *new_tape = realloc(tape, 2 * tape_len * sizeof(uint8_t));
    if (new_tape == NULL) {
        fprintf(stderr, "Out of memory!\n");
    }

    // zero out the new part of the tape
    uint8_t *right_middle = new_tape + tape_len;
    memset(right_middle, 0, tape_len);

    return new_tape;
}

bf_program *parse_file(FILE *file, int64_t size) {
    size_t len = 0; //Actual code length (comments removed)
    char *contents = malloc(sizeof(char) * (size + 1));
    contents[len] = '\0';
    len++;
    for (int i = 0; i < size; i++) {
        char c = (char) getc(file);
        switch (c) {
            case '+':
            case '-':
            case '[':
            case ']':
            case '.':
            case ',':
            case '<':
            case '>':
                contents[len] = c;
                len++;
                break;
            default:
                break;
        }
    }
    fclose(file);

    char *source = realloc(contents, (len + 2) * sizeof(char));
    source[len] = '\0'; // Zero-pad end

    bf_program *program = malloc(sizeof(bf_program));
    program->length = len + 1;
    program->source = source;
    return program;
}
