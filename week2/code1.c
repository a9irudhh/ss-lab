#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INST 100

typedef struct {
    int loc;
    char label[10];
    char opcode[10];
    char operand[10];
} Instruction;

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
    int startAddress = 0;  // To store starting address
    while (fgets(line, sizeof(line), file)) {
        char *label = strtok(line, " \t");
        char *opcode = strtok(NULL, " \t");
        char *operand = strtok(NULL, " \n");

        if (label != NULL)
            strcpy(inst[i].label, label);
        else
            strcpy(inst[i].label, "");

        if (opcode != NULL)
            strcpy(inst[i].opcode, opcode);
        else
            strcpy(inst[i].opcode, "");

        if (operand != NULL)
            strcpy(inst[i].operand, operand);
        else
            strcpy(inst[i].operand, "");

        if (strcmp(inst[i].opcode, "START") == 0) {
            memoryCounter = strtol(inst[i].operand, NULL, 16); 
            startAddress = memoryCounter;
        } 

        inst[i].loc = memoryCounter;

        if (strcmp(inst[i].opcode, "RESW") == 0) {
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
        } else if (strcmp(inst[i].opcode, "START") != 0) {
            memoryCounter += 3;
        }

        i++;
    }

    fclose(file);

    // Display the stored instructions
    printf("Instructions:\n");
    printf("Location\tLabel\tOpcode\tOperand\n");
    for (int j = 0; j < i; j++) {
        printf("%04X\t%s\t%s\t%s\n", inst[j].loc, inst[j].label, inst[j].opcode, inst[j].operand);
    }

    printf("\nFinal Memory Counter: %04X\n", memoryCounter);

    return 0;
}
