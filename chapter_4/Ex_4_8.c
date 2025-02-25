/*Suppose that there will never be more than one character of pushback. Modify
getch and ungetch accordingly.*/
include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSTACK 100  
#define NUMBER '0' 

void push(double value);
double pop();
int getop(char s[]);
int getch();
void ungetch(int c);
double stack[MAXSTACK];
int stackPos = 0;

int buf = EOF;

void push(double value) {
    if (stackPos < MAXSTACK) {
        stack[stackPos++] = value;
    } else {
        printf("Error: Stack full, cannot push %g\n", value);
    }
}

double pop() {
    if (stackPos > 0) {
        return stack[--stackPos];
    } else {
        printf("Error: Stack empty\n");
        return 0.0;
    }
}

int getop(char s[]) {
    int i = 0, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t');
    s[1] = '\0';

    if (!isdigit(c) && c != '.' && c != '-') {
        return c;
    }

    if (c == '-') {
        int next = getch();
        s[++i] = next;
        if (!isdigit(s[i]) && next != '.') {
            ungetch(next);
            return '-';
        }
        c = next;
    }

    if (isdigit(c)) {
        s[++i] = c = getch();
        while (isdigit(c)) {
            s[++i] = c = getch();
        }
    }

    if (c == '.') {
        s[++i] = c = getch();
        while (isdigit(c)) {
            s[++i] = c = getch();
        }
    }

    s[i] = '\0';
    if (c != EOF) {
        ungetch(c);
    }

    return NUMBER;
}

int getch() {
    if (buf != EOF) {
        int temp = buf;
        buf = EOF;
        return temp;
    } else {
        return getchar();
    }
}

void ungetch(int c) {
    if (buf != EOF) {
        printf("Pushback buffer already contains a character\n");
    } else {
        buf = c;
    }
}

int main() {
    char s[100];
    int type;
    double op2;

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '-':
                op2 = pop();
                 push(pop() - op2);
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0) {
                    push(pop() / op2);
                } else {
                    printf("Error: Division by zero\n");
                }
                break;
            case '%':
                op2 = pop();
                if (op2 != 0.0)
                    push((int)pop() % (int)op2);
                else
                    printf("Error: Zero divisor for modulus\n");
                break;

            case '\n':
                printf("Result: %.8g\n", pop());
                break;
            default:
                printf("Error: Unknown command %s\n", s);
                break;
        }
    }
    return 0;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                   1,1           Top

