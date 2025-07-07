/*An alternate organization uses getline to read an entire input line; this makes
getch and ungetch unnecessary. Revise the calculator to use this approach.*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXOP 100  
#define NUMBER '0'  
#define MAXLINE 1000 

char line[MAXLINE]; 
int line_pos = 0;   

int getop(char []);
void push(double);
double pop(void);
int getline_input(char[], int);

int main() {
    int type;
    double op2;
    char s[MAXOP];

    while (getline_input(line, MAXLINE) > 0) { 
        line_pos = 0; 
        while ((type = getop(s)) != '\0') {  
            switch (type) {
                case NUMBER:
                    push(atof(s));
                    break;
                case '+':
                    push(pop() + pop());
                    break;
                case '*':
                    push(pop() * pop());
                    break;
                case '-':
                    op2 = pop();
                    push(pop() - op2);
                    break;
                case '/':
                    op2 = pop();
                    if (op2 != 0.0)
                        push(pop() / op2);
                    else
                        printf("Division by zero\n");
                    break;
                case '\n':
                    printf("\t%.8g\n", pop());
                    break;
                default:
                    printf("Unknown command %s\n", s);
                    break;
            }
        }
    }
    return 0;
}
#define MAXVAL 100
int sp = 0;
double val[MAXVAL];

void push(double f) {
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("tack full\n");
}

double pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("Stack empty\n");
        return 0.0;
    }
}

int getline_input(char s[], int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    printf("%d \n",i);
    s[i] = '\0';
    return i;
}
int getop(char s[]) {
    int i = 0, c;

    while ((s[0] = c = line[line_pos++]) == ' ' || c == '\t')
        ;
    s[1] = '\0';

    if (c == '\0') return '\0';  

    if (!isdigit(c) && c != '.') return c;  

    if (isdigit(c)) 
        while (isdigit(s[++i] = c = line[line_pos++]))
            ;
    if (c == '.') 
        while (isdigit(s[++i] = c = line[line_pos++]))
            ;

    s[i] = '\0';
    line_pos--; 
    return NUMBER;
}
/*3 5 +
  8
  */
