/ *Write a function reverse(s) that reverses the character string s. Use it to
write a program that reverses its input a line at a time.*/
#include <stdio.h>

#define MAX_LINE 1000
void reverse(char *s) {
    int i = 0, j = 0;
    char temp;
    while (s[j] != '\0') {
        j++;
    }
    j--;
    while (i < j) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
        i++;
        j--;
    }
}
int get_line(char line[], int max_length) {
    int c, i = 0;

    while ((c = getchar()) != EOF && c != '\n') {
        if (i < max_length - 1) {
            line[i] = c;
        }
        i++;
    }
    if (i < max_length - 1) {
        line[i] = '\0';
    } else {
        line[max_length - 1] = '\0';
    }
    return i; 
}
int main() {
    char line[MAX_LINE]; 
    int len;
    while ((len = get_line(line, MAX_LINE)) > 0) {
        reverse(line);
        printf("%s\n", line);
    }
    
    return 0;
}

/* I am Priyanka
aknayirP ma I
*/
