//Write a program that prints its input one word per line
#include <stdio.h>
#include <ctype.h>

int main() {
    int c, flag = 0;

    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            if (flag) {
                putchar('\n');
                flag = 0;
            }
        } else {
            putchar(c);
            flag = 1;
        }
    }

    return 0;
}

/*my name is happy
my
name
is
happy
*/
