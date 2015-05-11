#include "vm.h"

/** testing addition */
int test_a[] = {
    PSH, 5,
    PSH, 2,
    ADD,
    PSH, 10,
    PSH, 12,
    ADD,
    ADD,
    POP,
    HLT
};

/** testing multiplication */
int test_b[] = {
    PSH, 5,
    PSH, 2,
    MUL,
    PSH, 10,
    PSH, 20,
    ADD,
    ADD,
    HLT
};

/** testing set and move */
int instructions[] = {
    SET, A, 21,     // 3
    GLD, A,         // 5
    PSH, 1,         // 7
    SUB,            // 8
    GPT, A,         // 10
    IFN, A, 0, 2,   // 14
    LOG, A,         // 16
    LOG, B,
    HLT             // 17
};


int simple_test[] = {
    PUSH, 1,
    PUSH, 2,
    ADD,
    POP,
    HALT
};

int main(int argc, char const *argv[]) {
    assert(evaluate(test_a) == 29);
    assert(evaluate(test_b) == 40);

    return 0;
}
