#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//#include "vm.h"

typedef struct {char* name; int val;} Entry;

typedef struct {
    int opcode;
    int args[3];
} Opcode;

typedef struct LLNode {
    struct LLNode* next;
    Opcode* val;
} LLNode;

typedef struct { LLNode *head, *tail; } LL;


LL* ll_create(void);
void ll_free(LL* ll, void(*freer)(void*));
void append(LL* n, Opcode* val);


int find(Entry map[], char* ident);

Instruction identify_instruction(char* ident);
        int identify_register   (char* ident);


Opcode* parse_opcode(Instruction instr, char* line);
char* lstrip(char* str);

LL* parse_opcodes(FILE* file);

void write_out(LL* ll, FILE* file);

#endif

