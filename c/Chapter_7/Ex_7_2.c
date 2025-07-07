/*Write a program that will print arbitrary input in a sensible way. As a minimum,
it should print non-graphic characters in octal or hexadecimal according to local custom, and
break long text lines.*/
#include <stdio.h>
#include <ctype.h>
#define MAX_WIDTH 20 
int main() {
    int c, col = 0; 
    while ((c = getchar()) != EOF) {
        if (isprint(c)) { 
            putchar(c);
            col++;
        } else {  
            printf("\\x%02X", c);
            col += 4;
        }
        if (col >= MAX_WIDTH) {
            putchar('\n');
            col = 0;
        }
    }
    return 0;
}
/*my name is      priyanka p
my name\x09is \x09pr
iyanka p\x0A*/
