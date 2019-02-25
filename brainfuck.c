#include "brainfuck.h"

int main() {
    FILE *file;
    file = fopen("test.bf", "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", "todo");
        exit(1);
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error reading file: %s\n", "todo");
        exit(1);
    }
    int64_t fsize = ftell(file);
    if (fsize == -1) {
        fprintf(stderr, "Error reading file: %s\n", "todo");
        exit(1);
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error reading file: %s\n", "todo");
        exit(1);
    }

    bf_program *program = parse_file(file, fsize);

    printf("%s\n", program->source);

    return 0;
}


bf_program *parse_file(FILE *file, int64_t size) {
    size_t len = 0; //Actual code length (comments removed)
    char *contents = malloc(sizeof(char) * size);
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

    char *source = realloc(contents, (len + 1) * sizeof(char));
    source[len] = '\0'; // Zero-pad for debugging convenience (make it printable)

    bf_program *program = malloc(sizeof(bf_program));
    program->length = len;
    program->source = source;
    return program;
}
