//Write a loop equivalent to the for loop above without using && or ||.
#include <stdio.h>
#define LIM 10
int main() {
    int c;
    char s[LIM];
    int i = 0;
    

    while (i < LIM - 1) {  
        c = getchar();     
        if (c == '\n')
            break;

        if (c == EOF)
            break;

        s[i++] = c; 
    }
    s[i] = '\0'; 
    printf("\nString: %s\n", s);

//    while ((c = getchar()) != EOF) {
//      putchar(c); 
// }
    return 0;
}

