//Write a program to copy its input to its output, replacing each string of one or
//more blanks by a single blank.
#include <stdio.h>

int main() {
    int c, prev;

    prev = 0; 

    printf("Enter text:\n");

    while ((c = getchar()) != EOF) {
        if (c != ' ' || prev != ' ') {
            putchar(c);  
        }
        prev = c;\
    }

    return 0;
}

/*Enter text:
hello world     !!! I am   Priyanka
hello world !!! I am Priyanka
 */
