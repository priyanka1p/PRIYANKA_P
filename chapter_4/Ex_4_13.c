//Write a recursive version of the function reverse(s), which reverses the
//string s in place.
#include <stdio.h>
#include <string.h>

void reversemain(char s[], int left, int right) {
    if (left >= right) 
        return; 
    char temp = s[left];
    s[left] = s[right];
    s[right] = temp;
    reversemain(s, left + 1, right - 1);
}

void reverse(char s[]) {
    int length = strlen(s);
    reversemain(s, 0, length - 1);
}

int main() {
    char s[] = "hello world";
    printf("Original: %s\n", s);
    
    reverse(s); 
    printf("Reversed: %s\n", s);
    return 0;
}

//Original: hello
//Reversed: olleh
