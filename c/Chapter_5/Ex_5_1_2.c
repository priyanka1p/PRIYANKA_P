/*As written, getint treats a + or - not followed by a digit as a valid
representation of zero. Fix it to push such a character back on the input.*/
#include <stdio.h>
#include <ctype.h>

#define SIZE 10 
#define BUFSIZE 100 

int getint(int *pn);
int getch(void);
void ungetch(int);
char buf[BUFSIZE];
int bufp = 0;    

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
int getint(int *pn) {
    int c, sign;

    while (isspace(c = getch()))
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-') {
        int next = getch();
        if (!isdigit(next)) {  
            ungetch(next);
            ungetch(c);
            return 0;
        }
        c = next;
    }

    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');

    *pn *= sign;

    if (c != EOF)
        ungetch(c);

    return c;
}

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
/*Enter integers (non-integer to stop):
78 67 456
-90 78 - 67
You entered: 78 67 456 -90 78 0 -703628560 31571 951322784 32764 
-
You entered: 0 0 0 0 0 0 155200240 32419 -287529360 32765 */
