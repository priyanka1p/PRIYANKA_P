//Write a program to print a histogram of the frequencies of different characters
//in its input.
#include <stdio.h>
#define RANGE 128

int main() {
    int c;
    int freq[RANGE] = {0};
    while ((c = getchar()) != EOF) {
        if (c < RANGE) {
            freq[c]++;
        }
    }

    printf("\n");
    for (int i = 0; i < RANGE; i++) {
        if (freq[i] > 0) {
            printf("'%c': ", (i >= 32 && i <= 126) ? i : ' ');
            for (int j = 0; j < freq[i]; j++) {
                printf("*");
            }
            printf("\n");}
    }
  return 0;
}


/* Indian Premiere League

' ': *
' ': **
'I': *
'L': *
'P': *
'a': **
'd': *
'e': *****
'g': *
'i': **
'm': *
'n': **
'r': **
'u': *
*/
