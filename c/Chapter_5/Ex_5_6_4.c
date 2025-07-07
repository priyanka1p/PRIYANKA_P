//reverse
#include <stdio.h>
void reverse_ptr(char *s) {
    char *end = s;
    while (*end) end++; 
    end--;
    while (s < end) {
        char temp = *s;
        *s++ = *end;
        *end-- = temp;
    }
}
int main() {
    char str[] = "hello";
    reverse_ptr(str);
    printf("Reversed: %s\n", str);
    return 0;
}
//olleh
