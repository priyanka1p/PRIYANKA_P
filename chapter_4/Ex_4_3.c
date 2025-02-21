/*Given the basic framework, it's straightforward to extend the calculator. Add the
modulus (%) operator and provisions for negative numbers.*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   

#define MAXOP 100  
#define NUMBER '0'  
#define MAXVAL 100  

int getop(char []);
void push(double);
double pop(void);

int sp = 0; 
double val[MAXVAL];  
void push(double f) {
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}
double pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}
int getop(char s[]) {
    int i = 0, c;
    
    while ((c = getchar()) == ' ' || c == '\t')
        ;

    s[i] = c;
    s[1] = '\0';

    if (!isdigit(c) && c != '.' && c != '-') 
        return c;

    if (c == '-') { 
        int next = getchar();
        if (!isdigit(next) && next != '.') {
            return c;
        }
        ungetc(next, stdin); 
    }

    while (isdigit(s[++i] = c = getchar()) || c == '.')
        ;

    s[i] = '\0';
    if (c != EOF)
        ungetc(c, stdin);

    return NUMBER;
}
int main() {
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
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
                    printf("error: zero divisor\n");
                break;
            case '%':
                op2 = pop();
                if (op2 != 0.0)
                    push((int)pop() % (int)op2);
                else
                    printf("error: zero divisor for modulus\n");
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    return 0;
}
/*-9 8 +
        -1
-99 9 +
        -90
9 7 %
        2*/
