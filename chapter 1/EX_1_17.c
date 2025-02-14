//Write a program to print all input lines that are longer than 80 characters.
#include <stdio.h>
#define MAX_LINE 1000  
#define THRESHOLD 80   
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
        if (len > THRESHOLD) {
            printf("Length: %d | Text: %s\n", len, line);
        }
    }
    return 0;
}

/*This is a short line.
This is a much longer line that should be printed because it exceeds 80 characters in length.
Another line.
Here is another line that also exceeds the 80-character threshold, so it should be displayed.
Length: 93 | Text: This is a much longer line that should be printed because it exceeds 80 characters in length.
Length: 93 | Text: Here is another line that also exceeds the 80-character threshold, so it should be displayed.
*/
