//book
#include <stdio.h>
#include <ctype.h>
#define SIZE 10 
#define BUFSIZE 100

int getch(void);
void ungetch(int);
int getint(int *pn);

char buf[BUFSIZE]; 
int bufp = 0;     

int getint(int *pn) {
    int c, sign;
    while (isspace(c = getch()))  // Skip white space
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
        c = getch();

    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');

    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    
    return c;
}

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
int main() {
    int array[SIZE], n;

    printf("Enter integers (non-integer to stop):\n");
    for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++)
        ; 

    printf("You entered: ");
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    
    printf("\n");
    return 0;
}
/*Enter integers (non-integer to stop):
78 67 45 -90
-
789
h
You entered: 78 67 45 -90 0 789 -38184208 29140 -1364839280 32764 */
