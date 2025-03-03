#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000 

char *lineptr[MAXLINES];  

int readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int nlines, int reverse);
void qsort_custom(void *v[], int left, int right, int (*comp)(void *, void *), int reverse);
int numcmp(const char *s1, const char *s2);


int main(int argc, char *argv[]) {
    int nlines;  
    int numeric = 0; 
    int reverse = 0; 


    while (--argc > 0 && (*++argv)[0] == '-') {
        char c;
        while ((c = *++argv[0])) {
            if (c == 'n') numeric = 1;
            else if (c == 'r') reverse = 1;
            else {
                printf("Invalid option: -%c\n", c);
                return 1;
            }
        }
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        qsort_custom((void **)lineptr, 0, nlines - 1, 
                     (int (*)(void *, void *))(numeric ? numcmp : strcmp), reverse);
        writelines(lineptr, nlines, reverse);
        return 0;
    } else {
        printf("Error: input too big to sort\n");
        return 1;
    }
}

void qsort_custom(void *v[], int left, int right, int (*comp)(void *, void *), int reverse) {
    int i, last;
    void swap(void *v[], int i, int j);

    if (left >= right) return;

    swap(v, left, (left + right) / 2);
    last = left;
    
    for (i = left + 1; i <= right; i++) {
        if ((reverse ? (*comp)(v[i], v[left]) > 0 : (*comp)(v[i], v[left]) < 0))
            swap(v, ++last, i);
    }

    swap(v, left, last);
    qsort_custom(v, left, last - 1, comp, reverse);
    qsort_custom(v, last + 1, right, comp, reverse);
}

int numcmp(const char *s1, const char *s2) {
    double v1 = atof(s1);
    double v2 = atof(s2);
    return (v1 > v2) - (v1 < v2); 
}

void swap(void *v[], int i, int j) {
    void *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int readlines(char *lineptr[], int maxlines) {
    int len, nlines = 0;
    char *p, line[1000];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        len = strlen(line);
        if (line[len - 1] == '\n') line[len - 1] = '\0';

        p = malloc(len);
        if (p == NULL) return -1;
        strcpy(p, line);
        lineptr[nlines++] = p;

        if (nlines >= maxlines) return -1;
    }
    return nlines;
}


void writelines(char *lineptr[], int nlines, int reverse) {
    for (int i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
}
