//find the longest line
#include <stdio.h>
#define MAXLINE 1000 
int my_getline(char line[], int maxline);
void copy(char to[], char from[]);
int main() {
    int len;
    int max = 0;
    char line[MAXLINE];
    char longest[MAXLINE];
    while ((len = my_getline(line, MAXLINE)) > 0) {
        if (len > max) {
            max = len;
            copy(longest, line);
        }
    }
    if (max > 0)
        printf("Longest line:\n%s", longest);
    return 0;
}
int my_getline(char s[], int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

void copy(char to[], char from[]) {
    int i = 0;
    while ((to[i] = from[i]) != '\0')
        ++i;
}
/* There are seven wonders in the world.
One among them is Taj Mahal.
It is built by Shah Jahan on memorial of his wife.
Longest line:
It is built by Shah Jahan on memorial of his wife*/
