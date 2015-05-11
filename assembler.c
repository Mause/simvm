#include "vm.h"

#define EQ(a, b) (strcmp(a, b) == 0)

typedef struct {
    int opcode;
    int args[3];
} Opcode;

typedef struct LLNode {
    struct LLNode* next;
    Opcode* val;
} LLNode;

typedef struct { LLNode *head, *tail; } LL;

LL* ll_create(void) {
    LL* ll = malloc(sizeof(LL));
    ll->head = ll->tail = NULL;
    return ll;
}

void append(LL* n, Opcode* val) {
    LLNode *new_node, *old_tail;

    new_node = malloc(sizeof(*new_node));
    new_node->val = val;

    if (n->head == NULL && n->tail == NULL) {
        n->head = n->tail = new_node;
    }

    old_tail = n->tail;
    n->tail = new_node;
    old_tail->next = new_node;
}


Instruction identify_instruction(char* ident) {
         if (EQ(ident, "ADD")) return ADD;
    else if (EQ(ident, "GLD")) return GLD;
    else if (EQ(ident, "GPT")) return GPT;
    else if (EQ(ident, "IFN")) return IFN;
    else if (EQ(ident, "LOG")) return LOG;
    else if (EQ(ident, "MUL")) return MUL;
    else if (EQ(ident, "NOP")) return NOP;
    else if (EQ(ident, "POP")) return POP;
    else if (EQ(ident, "SET")) return SET;
    else if (EQ(ident, "SUB")) return SUB;
    else if (EQ(ident, "HLT") || EQ(ident, "HALT")) return HALT;
    else if (EQ(ident, "PSH") || EQ(ident, "PUSH")) return PUSH;
    else return -1;
}


static int arg_nums[] = {
  // ADD, SUB, MUL, LOG, SET, GLD, GPT, POP, PUSH, HALT, IFN, NOP
     0,   0,   0,   1,   2,   1,   1,   0,   1,    0,    3,   0
};

Opcode* parse_opcode(Instruction instr, char* line) {
    Opcode* opcode;
    char buffer[1024];

    opcode = malloc(sizeof(*opcode));
    opcode->opcode = -1;
    memset(opcode->args, -1, 3);

    switch(instr) {
        case ADD: case SUB: case MUL: case POP: case NOP: case HALT: {
            opcode->opcode = instr;
            break;
        }
        case GLD: case GPT: {
            opcode->opcode = instr;
            assert(sscanf(line, "%s %c", buffer, &(opcode->args[0])) == 2);
            memset(opcode->args + 1, -1, 2);
            break;
        }
        case PUSH: {
            opcode->opcode = instr;
            assert(sscanf(line, "%s %d", buffer, &(opcode->args[0])) == 2);
            memset(opcode->args + 1, -1, 2);
            break;
        }
        case IFN: {
            opcode->opcode = instr;
            assert(sscanf(
                line,
                "%s %c %d %d",
                buffer,
                &(opcode->args[0]),
                &(opcode->args[1]),
                &(opcode->args[2])
            ) == 4);
            break;
        }

        default: assert(0 == 1);
    }

    return opcode;
}

LL* parse_opcodes(FILE* file) {
    int i_ident;
    char line[1024];
    LL* ll = ll_create();

    while (fgets(line, (size_t)1024, file) != NULL) {
        char* opcode = malloc(sizeof(char) * strlen(line));

        assert(sscanf(line, "%s", opcode) == 1);

        i_ident = identify_instruction(opcode);
        assert(i_ident != -1);

        append(
            ll,
            parse_opcode(i_ident, line)
        );
        printf(
            "%s(%d) %d %d %d\n",
            opcode, i_ident,
            ll->tail->val->args[0],
            ll->tail->val->args[1],
            ll->tail->val->args[2]
        );
    }

    return ll;
}


void write_out(LL* ll, FILE* file) {
    LLNode* current;
    int i;

    current = ll->head;

    while (current != NULL) {
        fputc(current->val->opcode, file);

        for(i=0; i<arg_nums[current->val->opcode]; i++) {
            fputc(current->val->args[i], file);
        }

        current = current->next;
    }
}


int main(int argc, const char *argv[]) {
    char* outname;
    FILE *file, *outfile;
    LL* ll;

    if (argc != 2) {
        fprintf(stderr, "Invalid args\n");
        exit(-1);
    }

    file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Couldn't open file");
        return -1;
    }

    ll = parse_opcodes(file);

    if (argc == 3) {
        outname = argv[2];
    } else {
        outname = strtok(argv[1], ".");
        strcat(outname, ".bin");
    }

    outfile = fopen(outname, "wb");
    if (outfile == NULL) {
        perror("Couldn't open output file");
        return -1;
    }

    write_out(ll, outfile);

    return 0;
}
