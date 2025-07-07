/*Write a version of itoa that accepts three arguments instead of two. The third
argument is a minimum field width; the converted number must be padded with blanks on the
left if necessary to make it wide enough.*/
#include <stdio.h>
#include <string.h>
#include<limits.h>
void reverse(char s[]) {
    int i, j;
    char temp;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}
void itoa(int n, char s[], int width) {
    int i = 0, sign = n;
    
    if (n == INT_MIN) {  
        n = -(n + 1);    
    } else if (n < 0) {
        n = -n;
    }


    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if(sign==INT_MIN){
        s[0]+=1;
    }
    if (sign < 0)  
        s[i++] = '-';
    
    while (i < width)  
        s[i++] = ' ';
    
    s[i] = '\0';
    reverse(s);
}

int main() {
    char buffer[20];
    itoa(10, buffer, 6);
    printf("'%s'\n", buffer);

    return 0;
}
//'-2147483648'
