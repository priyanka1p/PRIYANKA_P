/* Write a pointer version of the function strcat that we showed in Chapter 2:
strcat(s,t) copies the string t to the end of s.*/
#include <stdio.h>
void strcat_ptr(char *s, const char *t);

int main() {
    char s[100] = "Hello, "; 
    char t[] = "World!";

    strcat_ptr(s, t);

    printf("Concatenated String: %s\n", s);
    return 0;
}

void strcat_ptr(char *s, const char *t) {
    while (*s)
        s++;

    while ((*s++ = *t++))
        ; 
}
//Concatenated String: Hello, World!
