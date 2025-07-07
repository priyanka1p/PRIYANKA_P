//strindex
#include <stdio.h>
int strindex_ptr(const char *s, const char *t) {
    const char *p, *q, *r;
    for (p = s; *p; p++) {
        for (q = p, r = t; *r && *q == *r; q++, r++);
        if (!*r) return p - s; 
    }
    return -1; // Not found
}
int main() {
    char str[] = "hello world";
    char sub[] = "world";
    printf("Index: %d\n", strindex_ptr(str, sub));
    return 0;
}
//Index: 6
