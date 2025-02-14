//Write a program to remove trailing blanks and tabs from each line of input, and
//to delete entirely blank lines.
#include <stdio.h>
#define MAX_LINE 1000 
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
int trim_trailing_whitespace(char line[]) {
    int i = 0;
    while (line[i] != '\0') {  
        i++;
    }
    i--;
    while (i >= 0 && (line[i] == ' ' || line[i] == '\t')) { 
        i--;
    }
    if (i >= 0) {  
        line[i + 1] = '\0';
        return 1;
    }
    return 0;
}
int main() {
    char line[MAX_LINE];
    int len;
    while ((len = get_line(line, MAX_LINE)) > 0) {
        if (trim_trailing_whitespace(line)) {
            printf("%s\n", line);
        }   }
    return 0;
}

//this is falooda  . 	
//this is falooda  .
