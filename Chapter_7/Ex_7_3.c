/*Revise minprintf to handle more of the other facilities of printf.*/
#include <stdio.h>
#include <stdarg.h>
void minprintf(char *fmt, ...)
{
    va_list ap; 
    char *p, *sval;
    int ival;
    unsigned int uval;
    double dval;
    char cval;
    void *pval;
    int width, precision;

    va_start(ap, fmt); 
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        width = 0;
        precision = -1;
        if (*++p == '*') {
            width = va_arg(ap, int);
            p++;
        } else {
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }
        }
        if (*p == '.') {
            p++;
            if (*p == '*') {
                precision = va_arg(ap, int);
                p++;
            } else {
                precision = 0;
                while (*p >= '0' && *p <= '9') {
                    precision = precision * 10 + (*p - '0');
                    p++;
                }
            }
        }

        switch (*p) {
            case 'd':
                ival = va_arg(ap, int);
                printf("%*.*d", width, precision, ival);
                break;
            case 'u': 
                uval = va_arg(ap, unsigned int);
                printf("%*.*u", width, precision, uval);
                break;
            case 'o': 
                ival = va_arg(ap, int);
                printf("%*.*o", width, precision, ival);
                break;
            case 'x': 
                ival = va_arg(ap, int);
                printf("%*.*x", width, precision, ival);
                break;
            case 'f': 
                dval = va_arg(ap, double);
                printf("%*.*f", width, precision, dval);
                break;
            case 'c': 
                cval = (char) va_arg(ap, int);
                printf("%*c", width, cval);
                break;
            case 's': 
                sval = va_arg(ap, char *);
                printf("%*.*s", width, (precision < 0) ? (int)strlen(sval) : precision, sval);
                break;
            case 'p': 
                pval = va_arg(ap, void *);
                printf("%p", pval);
                break;
            case '%': 
                putchar('%');
                break;
            default:
                putchar(*p);
                break;
        }
    }
    va_end(ap); 
}

int main() {
    minprintf("Hello, %s! Your age is %d and your score is %.2f. Hex: %x, Char: %c, Pointer: %p\n",
              "Priyanka", 21, 95.5, 255, 'A', (void *)main);
    minprintf("Width example: %10d\n", 123);
    minprintf("Precision example: %.5f\n", 3.14159);
    minprintf("Combined: %8.3f\n", 2.71828);
    char *str = "Hello, World!";
    printf("%*.*s\n", 10, 3, str);

    return 0;
}
/*Hello, Priyanka! Your age is 21 and your score is 95.50. Hex: ff, Char: A, Pointer: 0x587a6b558898
Width example:        123
Precision example: 3.14159
Combined:    2.718
       Hel
*/
