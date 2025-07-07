/*Write a function htoi(s), which converts a string of hexadecimal digits
(including an optional 0x or 0X) into its equivalent integer value. The allowable digits are 0
through 9, a through f, and A through F.*/
#include <stdio.h>
#include <ctype.h>
int htoi(const char s[]) {
    int i = 0, n = 0, digit;
    if (s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
        i += 2;
    }
    while (s[i] != '\0') {
        if (isdigit(s[i])) {
            digit = s[i] - '0';
        } else if (s[i] >= 'a' && s[i] <= 'f') {
            digit = s[i] - 'a' + 10;
        } else if (s[i] >= 'A' && s[i] <= 'F') {
            digit = s[i] - 'A' + 10;
        } else {
            break;
        }

        n = 16 * n + digit;
        i++;
    }
    return n;
}
int main() {
    char hex1[] = "0x1A3F";
    printf("%s -> %d\n", hex1, htoi(hex1)); 
    return 0;
}
//0x1A3F -> 6719
