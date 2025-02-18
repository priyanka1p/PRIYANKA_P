/*Write a function expand(s1,s2) that expands shorthand notations like a-z in
the string s1 into the equivalent complete list abc...xyz in s2. Allow for letters of either case
and digits, and be prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that a
leading or trailing - is taken literally.*/
#include <stdio.h>
#include <ctype.h>
void expand(const char s1[], char s2[]) {
    int i, j = 0;
    char start, end;
    for (i = 0; s1[i] != '\0'; i++) {
        if (s1[i] == '-' && i > 0 && s1[i + 1] != '\0' && isalnum(s1[i - 1]) && isalnum(s1[i + 1]) && s1[i - 1] < s1[i + 1]) {
            start = s1[i - 1] + 1;
            end = s1[i + 1];
            while (start <= end) {
                s2[j++] = start++;
            }
            i++;
        } else {
            s2[j++] = s1[i];
        }
    }
    s2[j] = '\0';
}

int main() {
    char s1[] = "a-d0-3X-Z";
    char s2[100];
    expand(s1, s2);
    printf(" %s\n", s2);
    return 0;
}
// abcd0123XYZ
