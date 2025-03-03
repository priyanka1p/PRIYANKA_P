/*Write a program that converts upper case to lower or lower case to upper,
depending on the name it is invoked with, as found in argv[0].*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int (*convert)(int) = NULL;  
    int c;
    if (strstr(argv[0], "lower")) {
        convert = tolower;
    } else if (strstr(argv[0], "upper")) {
        convert = toupper;
    } else {
        fprintf(stderr, "rename program as 'lower' or 'upper'\n");
        return 1;
    }
    while ((c = getchar()) != EOF) {
        putchar(convert(c));
    }

    return 0;
}
/*user@user-Latitude-3420:~/Documents/chapter_5$ gcc ex_7_1.c -o lower
user@user-Latitude-3420:~/Documents/chapter_5$ ./lower
HEllo WorLd
hello world
*/
