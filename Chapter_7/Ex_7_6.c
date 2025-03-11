//Write a program to compare two files, printing the first line where they differ.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 1000  
void compare_files(FILE *fp1, FILE *fp2) {
    char line1[MAXLINE], line2[MAXLINE];
    int lineno = 1;
    while (fgets(line1, MAXLINE, fp1) != NULL && fgets(line2, MAXLINE, fp2) != NULL){
        if (strcmp(line1, line2) != 0) { 
            printf("Files differ at line %d:\n", lineno);
            printf("File1: %s", line1);
            printf("File2: %s", line2);
            return;
        }
        lineno++;
    }
    if (fgets(line1, MAXLINE, fp1) != NULL) {
        printf("File1 has extra content at line %d:\n%s", lineno, line1);
    } else if (fgets(line2, MAXLINE, fp2) != NULL) {
        printf("File2 has extra content at line %d:\n%s", lineno, line2);
    } else {
        printf("Files are identical.\n");
    }
}
int main(int argc, char *argv[]) {
    FILE *fp1, *fp2;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        exit(1);
    }
    if ((fp1 = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
        exit(1);
    }
    if ((fp2 = fopen(argv[2], "r")) == NULL) {
        fprintf(stderr, "Error: Cannot open %s\n", argv[2]);
        fclose(fp1);
        exit(1);
    }
    compare_files(fp1, fp2);
    fclose(fp1);
    fclose(fp2);

    return 0;
}
