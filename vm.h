#ifndef VM_H
#define VM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define STACK_SIZE 32
#define MEMORY_SIZE 1024

enum REGISTER {
    A, B, C, D, E, F, SP, PC, NUM_REGISTERS
};

typedef struct {
    int stack[STACK_SIZE];
    int* instructions;
    int registers[NUM_REGISTERS];
} VM;

typedef enum {
    ADD, SUB, MUL, LOG,
    SET, GLD, GPT, POP, PUSH,
    HALT, IFN, NOP
} Instruction;
#define HLT HALT
#define PSH PUSH

int fetch(VM* vm);
int pop(VM* vm);
void push(VM* vm, int val);
void execute(VM* vm);
VM* vm_create(void);
void print_stack(VM* vm);


#endif
