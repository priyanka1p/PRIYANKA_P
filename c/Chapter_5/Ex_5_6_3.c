//itoa
#include <stdio.h>

void reverse(char *s) {
    char *end = s;
    while (*end) end++;
    end--; 
    while (s < end) {
        char temp = *s;
        *s++ = *end;
        *end-- = temp;
    }
}
void itoa_ptr(int n, char *s) {
    char *p = s;
    int sign = n;

    if (n < 0) n = -n;
    
    do {
        *p++ = (n % 10) + '0'; 
    } while ((n /= 10) > 0);

    if (sign < 0) *p++ = '-';
    *p = '\0';

    reverse(s); 
}

int main() {
    char buffer[20];
    itoa_ptr(-1234, buffer);
    printf("Integer: -1234, String: %s\n", buffer);
    return 0;
}
