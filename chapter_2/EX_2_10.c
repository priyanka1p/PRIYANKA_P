//Rewrite the function lower, which converts upper case letters to lower case,
//with a conditional expression instead of if-else.
#include <stdio.h>
int lower(int c) {
    return (c >= 'A' && c <= 'Z') ? (c + 'a' - 'A') : c;
}

int main() {
    char uppercase = 'G';
    char lowercase = lower(uppercase);
    
    printf("Uppercase: %c, Lowercase: %c\n", uppercase, lowercase);

    return 0;
}
//Uppercase: G, Lowercase: g
