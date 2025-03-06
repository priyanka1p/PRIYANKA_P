/*Write a private version of scanf analogous to minprintf from the previous
section.*/
#include <stdio.h>
#include <stdarg.h>

void minscanf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const char *p;
    int *ip;
    float *fp;
    char *sp;

    for (p = format; *p; p++) {
        if (*p != '%') {
            continue; 
        }

        switch (*++p) {
            case 'd': 
                ip = va_arg(args, int *);
                scanf("%d", ip);
                break;
            case 'f':  
                fp = va_arg(args, float *);
                scanf("%f", fp);
                break;
            case 's':
                sp = va_arg(args, char *);
                scanf("%s", sp);
                break;
            default:
                printf("Unsupported format: %%%c\n", *p);
        }
    }

    va_end(args);
}

int main() {
    int num;
    float fnum;
    char str[50];

    printf("Enter an integer, a float, and a word: ");
    minscanf("%d %f %s", &num, &fnum, str);

    printf("You entered: %d, %.2f, %s\n", num, fnum, str);
    return 0;
}

/*Enter an integer, a float, and a word: priyanka
You entered: 0, 0.00, priyanka
Enter an integer, a float, and a word: 10.23 
2
You entered: 10, 0.23, 2
  Enter an integer, a float, and a word: 5 10.23 priyanka
You entered: 5, 10.23, priyanka
*/

