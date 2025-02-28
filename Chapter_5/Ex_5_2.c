/*Write getfloat, the floating-point analog of getint. What type does getfloat
return as its function value?*/
#include <stdio.h>
#include <ctype.h>

#define SIZE 10  
#define BUFSIZE 100 

int getfloat(float *pn);
int getch(void);
void ungetch(int);

char buf[BUFSIZE];  
int bufp = 0;     

int main() {
    float array[SIZE];
    int n;

    printf("Enter floating-point numbers (non-number to stop):\n");
    for (n = 0; n < SIZE && getfloat(&array[n]) != EOF; n++)
        ; 

    printf("You entered: ");
    for (int i = 0; i < n; i++)
        printf("%g ", array[i]);

    printf("\n");
    return 0;
}

int getfloat(float *pn) {
    int c, sign;
    float power = 1.0;

    while (isspace(c = getch()))
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-') {
        int next = getch();
        if (!isdigit(next) && next != '.') { 
            ungetch(next);
            ungetch(c);
            return 0;
        }
        c = next;
    }

    for (*pn = 0.0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');

    if (c == '.') {
        while (isdigit(c = getch())) {
            *pn = 10 * *pn + (c - '0');
            power *= 10.0;
        }
    }

    *pn = (*pn * sign) / power; 

    if (c != EOF)
        ungetch(c);

    return c;
}

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}

/*Enter floating-point numbers (non-number to stop):
3.4 2.2 5.6 33 .34 44 - 23.3
You entered: 3.4 2.2 5.6 33 0.34 44 6.84786e+06 4.55758e-41 6.69074e-16 4.59121e-41 
*/
