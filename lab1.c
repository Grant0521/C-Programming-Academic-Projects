//Grant Johnson, COEN12, 9/20/23
//Word count of text file

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char *filename = argv[1];
    char word[100];
    int wordCount = 0; // Defines Variables

    if (argc != 2) {
        printf("Error: Invalid Number of File Arguments");
        return 1; // Provides error if >1 filename is given
    }

    file = fopen(filename, "r"); // Opens file 

    if (file == NULL) {
        printf("File not found or unable to open.\n");
        return 1; // Provides error if file cannot open
    } 

    while (fscanf(file, "%s", word) != EOF) { // Increases word count until file ends
        wordCount++;
    } 

    fclose(file); // Closes files

    printf("%d total words \n", wordCount); // Outputs wordcount

    return 0;
}
