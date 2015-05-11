#include "vm.h"


int fetch(VM* vm) {
    return vm->instructions[vm->registers[PC]++];
}


int pop(VM* vm) {
    int val = vm->stack[vm->registers[SP]];

    vm->stack[vm->registers[SP]] = 0;
    vm->registers[SP]--;

    return val;
}


void push(VM* vm, int val) {
    vm->stack[++vm->registers[SP]] = val;
}


void execute(VM* vm) {
    int inst, halt = 0;
    while (!halt) {
        inst = fetch(vm);

        switch(inst) {
            case ADD:  push(vm, pop(vm) + pop(vm)); break;
            case SUB:  push(vm, pop(vm) - pop(vm)); break;
            case MUL:  push(vm, pop(vm) * pop(vm)); break;
            case PUSH: push(vm, fetch(vm));         break;
            case GLD:  push(vm, vm->registers[fetch(vm)]);   break;
            case GPT:  vm->registers[fetch(vm)] = pop(vm);   break;
            case SET:  vm->registers[fetch(vm)] = fetch(vm); break;
            case IFN: {
                if (vm->registers[fetch(vm)] != fetch(vm)) {
                    vm->registers[PC       ] =  fetch(vm);
                }
                break;
            }
            case NOP:            break;
            case POP:  pop(vm);  break;
            case HALT: halt = 1; break;
            default: fprintf(stderr, "Unknown instruction at address %d\n", vm->registers[PC]); exit(-1);
        }
    }
}


VM* vm_create(void) {
    VM* vm = malloc(sizeof(VM));

    vm->registers[PC] = 0;
    vm->registers[SP] = -1;
    memset(vm->stack, 0, STACK_SIZE);

    return vm;
}


void print_stack(VM* vm) {
    int i;
    for (i=0; i<STACK_SIZE; i++) {
        printf("%d", vm->stack[i]);
        if (i != (STACK_SIZE - 1)) {
            printf(", ");
        }
    }
    printf("\n");
}


int evaluate(int instructions[]) {
    VM* vm;
    int val;

    vm = vm_create();
    vm->instructions = instructions;
    execute(vm);
    val = vm->stack[0];
    // print_stack(vm);
    free(vm);

    return val;
}
