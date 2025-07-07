/*In a two's complement number representation, our version of itoa does not
handle the largest negative number, that is, the value of n equal to -(2wordsize-1). Explain why not.
Modify it to print that value correctly, regardless of the machine on which it runs.*/
#include <stdio.h>
#include <limits.h>
#include<string.h>
void reverse(char s[]) {
    int i, j;
    char temp;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}
void itoa(int n, char s[]) {
    int i = 0, sign = n;
    
    if (n == INT_MIN) {  
        n = -(n + 1);    
    } else if (n < 0) {
        n = -n;
    }

    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) {
        if (sign == INT_MIN) {
            s[0] += 1;  
        }
        s[i++] = '-';
    }

    s[i] = '\0';
    reverse(s);
}

int main() {
    char str[20];
    itoa(-2147483648, str);
    printf("Converted: %s\n", str);

}

//Converted: -2147483648
