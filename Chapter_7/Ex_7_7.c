/*Modify the pattern finding program of Chapter 5 to take its input from a set of
named files or, if no files are named as arguments, from the standard input. Should the file
name be printed when a matching line is found?*/
#include <stdio.h>
#include <string.h>

#define MAXLINE 1000  

int getline_custom(char *line, int max, FILE *fp);
void search_pattern(FILE *fp, char *filename, char *pattern);
int main(int argc, char *argv[]) {
    char pattern[MAXLINE];
    FILE *fp;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pattern [file1 file2 ...]\n", argv[0]);
        return 1;
    }
    strcpy(pattern, argv[1]);
    if (argc == 2) {
        search_pattern(stdin, "stdin", pattern);
    } else {
        for(int i = 2; i < argc; i++) {
            if ((fp = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "Error: cannot open file %s\n", argv[i]);
                continue;
            }
            search_pattern(fp, argv[i], pattern);
            fclose(fp);
        }
    }
    return 0;
}
int getline_custom(char *line, int max, FILE *fp) {
    int c, i = 0;
    while (--max > 0 && (c = fgetc(fp)) != EOF && c != '\n')
        line[i++] = c;
    if (c == '\n')
        line[i++] = c;
    line[i] = '\0';
    return i;
}
void search_pattern(FILE *fp, char *filename, char *pattern) {
    char line[MAXLINE];
    int lineno = 0;
        while (getline_custom(line, MAXLINE, fp) > 0) {
        lineno++;
        if (strstr(line, pattern) != NULL) {
            if (filename)  
                printf("%s:%d: %s", filename, lineno, line);
            else
                printf("%s", line);
        }
    }
}
/*
hi i am priyanka
stdin:1: hi i am priyanka
i am from villupuram
priyanka is a universal team
stdin:4: priyanka is a universal team
priyanka chopra is an actress
stdin:5: priyanka chopra is an actress
skeleton structure 
*/
