/*Rewrite readlines to store lines in an array supplied by main, rather than
calling alloc to maintain storage. How much faster is the program?*/
#include <stdio.h>
#include <string.h>

#define MAXLINES 50
#define MAXLEN 20 
#define MAXSTORAGE 20

int my_getline(char *s, int lim);
int readlines(char *lineptr[], char *storage, int maxlines, int maxstorage);

int main() {
    char *lineptr[MAXLINES];
    char linestore[MAXSTORAGE];
    int nlines = readlines(lineptr, linestore, MAXLINES, MAXSTORAGE);
    if (nlines < 0) {
        printf("Error: too many lines or storage exceeded.\n");
        return 1;
    }

    printf("\nStored Lines:\n");
    for (int i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
    
    return 0;
}
int readlines(char *lineptr[], char *storage, int maxlines, int maxstorage) {
    int len, nlines = 0;
    char line[MAXLEN];
    char *p = storage;
    char *storage_end = storage + maxstorage;

    while ((len = my_getline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || p + len > storage_end)
            return -1;
        
        line[len - 1] = '\0';
        strcpy(p, line);
        lineptr[nlines++] = p;
        p += len;
    }
    return nlines;
}
int my_getline(char *s, int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}
/*i'm priyanka
i work

Stored Lines:
i'm priyanka
i work
*/
