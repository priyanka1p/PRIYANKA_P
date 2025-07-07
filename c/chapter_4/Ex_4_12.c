/* Adapt the ideas of printd to write a recursive version of itoa; that is, convert
an integer into a string by calling a recursive routine.*/
#include <stdio.h>
void itoa(int n, char s[]) {
    static int i = 0;
    if (n < 0) { 
        s[i++] = '-';
        n = -n;
    }
    if (n / 10)
        itoa(n / 10, s);
    s[i++] = (n % 10) + '0';
    s[i] = '\0'; 
}

int main() {
    char str[20];
    int num = -1234;
    itoa(num, str);
    printf("string: %s\n", str);
    return 0;
}
//string: -1234
