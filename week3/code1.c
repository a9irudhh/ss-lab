#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INST 100
#define MAX_OPTAB 24
#define MAX_SYMTAB 100

typedef struct {
    int loc;
    char label[10];
    char opcode[10];
    char operand[10];
} Instruction;

typedef struct {
    char label[10];
    int address;
} SymbolTable;

typedef struct {
    char opcode[10];
    int format;
    int opcodeValue; 
} OpTAB;

SymbolTable symtab[MAX_SYMTAB];
int symCount = 0;

OpTAB optab[MAX_OPTAB] = {
    {"LDA", 3, 0}, {"LDX", 3, 4}, {"LDS", 3, 108}, {"STA", 3, 12},
    {"STX", 3, 16}, {"LDT", 3, 116}, {"ADD", 3, 24}, {"SUB", 3, 28},
    {"MUL", 3, 32}, {"DIV", 3, 36}, {"COMP", 3, 40}, {"TIX", 3, 44},
    {"JEQ", 3, 48}, {"JGT", 3, 52}, {"JLT", 3, 56}, {"JSUB", 3, 60},
    {"RSUB", 3, 76}, {"LDCH", 3, 80}, {"STCH", 3, 84}, {"J", 3, 60},
    {"ADDR", 2, 144}, {"COMPR", 2, 160}, {"CLEAR", 2, 180}, {"MULR", 2, 168}
};

int opCount = MAX_OPTAB;

int searchSymbol(char *label) {
    for (int i = 0; i < symCount; i++) {
        if (strcmp(symtab[i].label, label) == 0) {
            return 1;
        }
    }
    return 0; 
}

void addSymbol(char *label, int loc) {
    if (searchSymbol(label)) {
        printf("Error: Duplicate label '%s' found.\n", label);
        return;
    }
    strcpy(symtab[symCount].label, label);
    symtab[symCount].address = loc;
    symCount++;
}

int isValidOpcode(char *opcode) {
    for (int i = 0; i < opCount; i++) {
        if (strcmp(optab[i].opcode, opcode) == 0) {
            return optab[i].format;
        }
    }
    
    if (strcmp(opcode, "START") == 0 || strcmp(opcode, "RESB") == 0 || 
        strcmp(opcode, "RESW") == 0 || strcmp(opcode, "BYTE") == 0 || 
        strcmp(opcode, "WORD") == 0 || strcmp(opcode, "END") == 0) {
        return -1;
    }
    
    printf("INVALID OPCODE: %s\n", opcode);
    return 0;
}


int main() {
    FILE *file;
    Instruction inst[MAX_INST];
    unsigned int memoryCounter = 0;
    char line[100];

    file = fopen("alp.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 0;
    }

    int i = 0;
    int startAddress = 0;

    while (fgets(line, sizeof(line), file)) {

        char *label = strtok(line, " \t");
        char *opcode = strtok(NULL, " \t");
        char *operand = strtok(NULL, " \n");

        if (strcmp(label, "-") != 0) {
            strcpy(inst[i].label, label);
            addSymbol(label, memoryCounter);
        } else {
            strcpy(inst[i].label, "");
        }

        if (opcode != NULL) {
            strcpy(inst[i].opcode, opcode);
        } else {
            strcpy(inst[i].opcode, "");
        }

        if (operand != NULL) {
            strcpy(inst[i].operand, operand);
        } else {
            strcpy(inst[i].operand, "");
        }

        if (strcmp(inst[i].opcode, "START") == 0) {
            memoryCounter = strtol(inst[i].operand, NULL, 16);
            startAddress = memoryCounter;
        }

        inst[i].loc = memoryCounter;


        int format = isValidOpcode(inst[i].opcode);
        if (format > 0) {
            memoryCounter += format;
        } else if (strcmp(inst[i].opcode, "RESW") == 0) {
            memoryCounter += 3 * strtol(inst[i].operand, NULL, 10);  
        } else if (strcmp(inst[i].opcode, "BYTE") == 0) {
            if (inst[i].operand[0] == 'C') {
                memoryCounter += strlen(inst[i].operand) - 3; 
            } else if (inst[i].operand[0] == 'X') {
                memoryCounter += (strlen(inst[i].operand) - 3) / 2;  
            }
        } else if (strcmp(inst[i].opcode, "RESB") == 0) {
            memoryCounter += strtol(inst[i].operand, NULL, 10);  
        } else if (strcmp(inst[i].opcode, "WORD") == 0) {
            memoryCounter += 3; 
        } else if (strcmp(inst[i].opcode, "END") == 0) {
            break;
        } else if (strcmp(inst[i].opcode, "START") != 0 && format == 0) {
            printf("Invalid opcode '%s' at line %d, skipping memory increment.\n", inst[i].opcode, i+1);
        }

        i++;
    }

    fclose(file);

    printf("\nInstructions:\n");
    printf("Loc\tLabel\tOpcode\tOperand\n");
    for (int j = 0; j < i; j++) {
        if (strcmp(inst[j].label, "") != 0)
            printf("%04X\t%s\t%s\t%s\n", inst[j].loc, inst[j].label, inst[j].opcode, inst[j].operand);
        else
            printf("%04X\t\t%s\t%s\n", inst[j].loc, inst[j].opcode, inst[j].operand);
    }

    printf("\nSymbol Table:\n");
    printf("Label\tAddress\n");
    for (int k = 0; k < symCount; k++) {
        printf("%s\t%04X\n", symtab[k].label, symtab[k].address);
    }

    printf("\nFinal Memory Counter: %04X\n", memoryCounter);

    return 0;
}
