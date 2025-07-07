/*Write an alternative version of squeeze(s1,s2) that deletes each character in
s1 that matches any character in the string s2.*/
#include <stdio.h>
void squeeze(char s1[],char s2[]) {
    int i, j, k;
    int match;
    for (i = j = 0; s1[i] != '\0'; i++) {
        match = 0;
        for (k = 0; s2[k] != '\0'; k++) {
            if (s1[i] == s2[k]) {
                match = 1;
                break;
            }
        }
        if (!match)
            s1[j++] = s1[i];
    }
    s1[j] = '\0';
}
int main() {
    
    char str1[20];
    char str2[] = "lo";
    fgets(str1, 20, stdin);
    squeeze(str1, str2);
    printf("Result: %s\n", str1);
    return 0;
}

//hello world
//Result: he wrd
