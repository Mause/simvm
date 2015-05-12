#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // for isspace

#include "vm.h"

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

static Entry REGISTER_MAP[] = {
    {"A", A}, {"B", B}, {"C", C}, {"D", D}, {"E", E}, {"F", F},
    {"SP", SP}, {"PC", PC}, {NULL, -1}
};

static Entry OPCODE_MAP[] = {
    {"ADD", ADD},  {"GLD", GLD},   {"GPT", GPT},
    {"IFN", IFN},  {"LOG", LOG},   {"MUL", MUL},
    {"NOP", NOP},  {"POP", POP},   {"SET", SET},
    {"SUB", SUB},  {"HLT", HALT},  {"HALT", HALT},
    {"PSH", PUSH}, {"PUSH", PUSH}, {NULL, -1}
};

LL* ll_create(void) {
    LL* ll = malloc(sizeof(*ll));
    ll->head = NULL;
    ll->tail = NULL;
    return ll;
}

void ll_free(LL* ll, void(*freer)(void*)) {
    LLNode *current, *next=ll->head;

    while (next != NULL) {
        current = next;
        next = next->next;
        freer(current->val);
        free(current);
    }
    free(ll);
}

void append(LL* n, Opcode* val) {
    LLNode *new_node, *old_tail;

    new_node = malloc(sizeof(*new_node));
    new_node->val = val;
    new_node->next = NULL;

    if (n->head == NULL && n->tail == NULL) {
        n->head = n->tail = new_node;
    }

    old_tail = n->tail;
    n->tail = new_node;
    old_tail->next = new_node;
}

int find(Entry map[], char* ident) {
    int i = 0;

    for (i=0; map[i].name != NULL; i++) {
        if (strcmp(map[i].name, ident) == 0) {
            return map[i].val;
        }
    }

    return -1;
}

Instruction identify_instruction(char* ident) { return find(OPCODE_MAP,   ident); }
        int identify_register   (char* ident) { return find(REGISTER_MAP, ident); }


static int OPCODE_ARG_NUMS[] = {
  // ADD, SUB, MUL, LOG, SET, GLD, GPT, POP, PUSH, HALT, IFN, NOP
     0,   0,   0,   1,   2,   1,   1,   0,   1,    0,    3,   0
};

Opcode* parse_opcode(Instruction instr, char* line) {
    Opcode* opcode;
    char buffer[1024];
    char registerr[3];

    opcode = malloc(sizeof(*opcode));
    opcode->opcode = -1;
    opcode->args[0] = -1;
    opcode->args[1] = -1;
    opcode->args[2] = -1;

    switch(instr) {
        case ADD: case SUB: case MUL: case POP: case NOP: case HALT: {
            opcode->opcode = instr;
            break;
        }
        case GLD: case GPT: {
            opcode->opcode = instr;
            assert(sscanf(line, "%s %s", buffer, registerr) == 2);
            opcode->args[0] = identify_register(registerr);
            if (opcode->args[0] == -1) {
                fprintf(stderr, "Invalid register: %s\n", registerr);
                return NULL;
            }
            break;
        }
        case PUSH: {
            opcode->opcode = instr;
            assert(sscanf(line, "%s %d", buffer, &(opcode->args[0])) == 2);
            break;
        }
        case IFN: {
            opcode->opcode = instr;
            assert(sscanf(
                line,
                "%s %s %d %d",
                buffer,
                registerr,
                &(opcode->args[1]),
                &(opcode->args[2])
            ) == 4);
            opcode->args[0] = identify_register(registerr);
            if (opcode->args[0] == -1) {
                fprintf(stderr, "Invalid register: %s\n", registerr);
                return NULL;
            }
            break;
        }

        default: assert(0 == 1);
    }

    return opcode;
}


// https://stackoverflow.com/questions/122616/
char* lstrip(char* str) {
    char *end;

    // Trim leading space
    while(isspace(*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}

LL* parse_opcodes(FILE* file) {
    int i_ident;
    char line[1024];
    LL* ll = ll_create();

    while (fgets(line, (size_t)1024, file) != NULL) {
        Opcode* val;
        char *opcode, *stripped;

        stripped = lstrip(line);
        if (stripped[0] == '#' || stripped[0] == '\0') { // comment or empty line
            continue;
        }

        opcode = malloc(sizeof(char) * strlen(line));
        assert(sscanf(stripped, "%s", opcode) == 1);

        i_ident = identify_instruction(opcode);
        assert(i_ident != -1);

        val = parse_opcode(i_ident, stripped);
        if (val == NULL) {
            printf("invalid line: %s\n", stripped);
            continue;
        }
        append(ll, val);
        printf(
            "%s(%d) %d %d %d\n",
            opcode, i_ident,
            val->args[0],
            val->args[1],
            val->args[2]
        );

        free(opcode);
    }

    return ll;
}


void write_out(LL* ll, FILE* file) {
    LLNode* current;
    int i;

    current = ll->head;

    while (current != NULL) {
        fputc(current->val->opcode, file);

        for(i=0; i<OPCODE_ARG_NUMS[current->val->opcode]; i++) {
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

    ll_free(ll, free);

    return 0;
}
