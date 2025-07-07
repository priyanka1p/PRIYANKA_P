/*Write the function itob(n,s,b) that converts the integer n into a base b
character representation in the string s. In particular, itob(n,s,16) formats s as a
hexadecimal integer in s.*?
#include <stdio.h>
#include <string.h>
void reverse(char s[]) {
    int i, j;
    char temp;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}
void itob(int n, char s[], int b) {
    int i = 0, sign = n;
    unsigned int num = (n < 0 && b == 10) ? -n : (unsigned int)n;

    do {
        int digit = num % b;
        s[i++] = (digit < 10) ? digit + '0' : digit - 10 + 'A';
    } while ((num /= b) > 0);

    if (sign < 0 && b == 10) {
        s[i++] = '-';
    }

    s[i] = '\0';
    reverse(s);
}
int main() {
    char buffer[50];
    itob(46, buffer, 2);  
    printf("Binary: %s\n", buffer);

    itob(2550, buffer, 8);  
    printf("Octal: %s\n", buffer);

    itob(-25, buffer, 10); 
    printf("Decimal: %s\n", buffer);

    itob(25, buffer, 16); 
    printf("Hexadecimal: %s\n", buffer);
    return 0;
}
/*Binary: 101110
Octal: 4766
Decimal: -25
Hexadecimal: 19
*/
