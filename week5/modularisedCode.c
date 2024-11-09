#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

struct OPTAB {
    char opcode[10];
    char code[10];
} optab[MAX_LEN];

struct SYMTAB {
    char symbol[10];
    char address[10];
} symtab[MAX_LEN];

int optabSize = 0, symtabSize = 0;

void loadOptab() {
    FILE *fp = fopen("optab.txt", "r");
    if (fp == NULL) {
        printf("Error opening optab.txt\n");
        exit(1);
    }
    while (fscanf(fp, "%s %s", optab[optabSize].opcode, optab[optabSize].code) != EOF) {
        optabSize++;
    }
    fclose(fp);
}

void loadSymtab() {
    FILE *fp = fopen("symtab.txt", "r");
    if (fp == NULL) {
        printf("Error opening symtab.txt\n");
        exit(1);
    }
    while (fscanf(fp, "%s %s", symtab[symtabSize].symbol, symtab[symtabSize].address) != EOF) {
        symtabSize++;
    }
    fclose(fp);
}

char* getOpcodeCode(char* opcode) {
    for (int i = 0; i < optabSize; i++) {
        if (strcmp(optab[i].opcode, opcode) == 0) {
            return optab[i].code;
        }
    }
    return NULL; 
}

char* getSymbolAddress(char* symbol) {
    for (int i = 0; i < symtabSize; i++) {
        if (strcmp(symtab[i].symbol, symbol) == 0) {
            return symtab[i].address;
        }
    }
    return NULL; 
}

void processInstruction(char *loc, char *label, char *opcode, char *operand, FILE *output) {
    char *opcodeCode = getOpcodeCode(opcode);  
    if (opcodeCode) {
        if (operand[0] != '\0') { 
            char *operandAddress = getSymbolAddress(operand); 
            if (operandAddress) {
                fprintf(output, "%s\t%s%s\n", loc, opcodeCode, operandAddress);  
            } else {
                fprintf(output, "%s\t%s0000\n", loc, opcodeCode); 
            }
        } else {
            fprintf(output, "%s\t%s0000\n", loc, opcodeCode);  
        }
    } else if (strcmp(opcode, "WORD") == 0) {
        fprintf(output, "%s\t%06X\n", loc, atoi(operand)); 
    } else if (strcmp(opcode, "BYTE") == 0) {
        if (operand[0] == 'C') {
            for (int i = 2; i < strlen(operand) - 1; i++) {
                fprintf(output, "%02X", operand[i]); 
            }
            fprintf(output, "\n");
        } else if (operand[0] == 'X') {
            for (int i = 2; i < strlen(operand) - 1; i++) {
                fprintf(output, "%c", operand[i]); 
            }
            fprintf(output, "\n");
        }
    }
}

void pass2() {
    FILE *intermediate = fopen("intermediate.txt", "r");
    FILE *output = fopen("output.txt", "w");
    if (intermediate == NULL || output == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    char loc[10], label[10], opcode[10], operand[10];
    char programName[10], startAddress[10], programLength[10];

    fscanf(intermediate, "%s %s %s %s", loc, label, opcode, operand);
    strcpy(programName, label);
    strcpy(startAddress, loc);

    while (fscanf(intermediate, "%s %s %s %s", loc, label, opcode, operand) != EOF) {
        if (strcmp(opcode, "END") == 0) {
            break;
        }
    }
    int start = (int)strtol(startAddress, NULL, 16);
    int end = (int)strtol(loc, NULL, 16);
    sprintf(programLength, "%06X", end - start);

    char paddedStartAddress[7], paddedProgramLength[7];

    snprintf(paddedStartAddress, sizeof(paddedStartAddress), "%06X", strtol(startAddress, NULL, 16));
    snprintf(paddedProgramLength, sizeof(paddedProgramLength), "%06X", strtol(programLength, NULL, 16));

    fprintf(output, "H%-6.6s%s%s\n", programName, paddedStartAddress, paddedProgramLength);
    rewind(intermediate);
    fscanf(intermediate, "%s %s %s %s", loc, label, opcode, operand);  // Skip the first line

    while (fscanf(intermediate, "%s %s %s %s", loc, label, opcode, operand) != EOF) {
        if (strcmp(opcode, "RSUB") == 0) {
            operand[0] = '\0'; 
        }
        processInstruction(loc, label, opcode, operand, output);
    }

    fclose(intermediate);
    fclose(output);
}

int main() {
    loadOptab();  
    loadSymtab(); 
    pass2();  
    printf("Pass 2 complete. Output generated in output.txt\n");
    return 0;
}
