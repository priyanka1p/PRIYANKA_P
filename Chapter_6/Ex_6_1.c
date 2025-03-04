/*Our version of getword does not properly handle underscores, string constants,
comments, or preprocessor control lines. Write a better version.*/
#include <stdio.h>
#include <ctype.h>
#include<string.h>
#define MAXWORD 100
#define BUFSIZE 100

struct key {
    char *word;
    int count;
} keytab[] = {
    {"auto", 0}, {"break", 0}, {"case", 0}, {"char", 0},
    {"const", 0}, {"continue", 0}, {"default", 0},{"int",0 {"unsigned", 0},
    {"void", 0}, {"volatile", 0}, {"while", 0}
};

#define NKEYS (sizeof keytab / sizeof(keytab[0]))

int getword(char *word, int lim);
int getch(void);
void ungetch(int);
int binsearch(char *, struct key *, int);

char buf[BUFSIZE];
int bufp = 0;

int main() {
    int n;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;

    for (n = 0; n < NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);

    return 0;
}

int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low = 0, high = n - 1, mid;

    while (low <= high) {
        mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

int getword(char *word, int lim) {
    int c;
    char *w = word;\
    while (isspace(c = getch()))
        ;
    if (c == '#') {
        while ((c = getch()) != '\n' && c != EOF) 
            ;
        return getword(word, lim); 
    }
    if (c == '\"') {
        while ((c = getch()) != '\"' && c != EOF) {
            if (c == '\\')
                getch();
        }
        return getword(word, lim);
    }

    if (c == '/') {
        int next = getch();
        if (next == '/') { 
            while ((c = getch()) != '\n' && c != EOF)
                ;
            return getword(word, lim);
        } else if (next == '*') { 
            while ((c = getch()) != EOF) {
                if (c == '*' && (next = getch()) == '/')
                    break;
            }
            return getword(word, lim);
        } else {
            ungetch(next); 
        }
    }

    if (c == EOF)
        return EOF;

    if (isalpha(c) || c == '_') {
        *w++ = c;
        for (; --lim > 0; w++) {
            if (!isalnum(*w = getch()) && *w != '_') {
                ungetch(*w);
                break;
            }
        }
        *w = '\0';
        return word[0];
    }
    *w = '\0';
    return c;
}
/*int
while
#define ninia jndja
// int while//
keyword
while
void
/* int while*/
   1 int
   1 void
   2 while*/
