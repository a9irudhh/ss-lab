#include <stdio.h>
#include <string.h>

typedef struct code2 {
    char name[20];
    char usn[20];
    char dept[20];
    float sgpa;
} Student;

int main() {
    FILE *file;
    Student students[10];

    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    int i = 0;
    while (i < 10 && fscanf(file, "%s %s %s %f", students[i].name, students[i].usn, students[i].dept, &students[i].sgpa) != EOF)
        i++;

    fclose(file);

    float sum = 0.0;
    for(int j = 0; j < 10; j++) 
        sum += students[j].sgpa;

    float avg = sum / 10; 

    printf("Student Records:\n");
    for (i = 0; i < 10; i++)
        if(students[i].sgpa > avg)
            printf("Name: %s, USN: %s, Department: %s, SGPA: %f\n", students[i].name, students[i].usn, students[i].dept, students[i].sgpa);

    return 0;
}
