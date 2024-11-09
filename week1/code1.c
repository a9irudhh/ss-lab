#include <stdio.h>
#include <string.h>

int main() {
    FILE *file, *output;
    char line[150];

    file = fopen("./log.txt", "r");
    output = fopen("./output.txt", "w");
    if (file == NULL || output == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        char actualline[10], extra[140];
        sscanf(line, "%s", actualline);

        if (strcmp("ERROR", actualline) == 0) {
            fprintf(output, "%s", line);
        }
    }

    fclose(file);
    fclose(output);

    return 0;
}
