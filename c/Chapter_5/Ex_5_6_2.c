//atoi
#include <stdio.h>
int atoi_ptr(const char *s) {
    int n = 0, sign = 1;
    while (*s == ' ' || *s == '\t' || *s == '\n')
        s++;
    if (*s == '-' || *s == '+') {
        if (*s == '-') sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9')
        n = 10 * n + (*s++ - '0');
    return sign * n;
}
int main() {
    char str[] = "-1234";
    printf("String: %s, Integer: %d\n", str, atoi_ptr(str));
    return 0;
}
