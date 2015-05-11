#include "vm.h"

int* read_from_file(FILE* file) {
    int *instructions, idx = 0, count = 0;

    while (fgetc(file) != EOF) count++;

    rewind(file);

    instructions = malloc(count * sizeof(int));

    for (idx=0; idx < count; idx++) {
        instructions[idx] = fgetc(file);
    }

    return instructions;
}


int main(int argc, const char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid args\n");
        return -1;

    } else {
        printf(
            "%d\n",
            evaluate(
                read_from_file(
                    fopen(argv[1], "rb")
                )
            )
        );

        return 0;
    }
}
