/*Write a routine ungets(s) that will push back an entire string onto the input.
Should ungets know about buf and bufp, or should it just use ungetch?*/
#include <stdio.h>
#include <stdlib.h>  
#include <ctype.h>  
#include <string.h>  

#define MAXSTACK 100  // Maximum stack size
#define MAXBUF 100    // Maximum buffer size
#define NUMBER '0'    // Signal that a number was found

void push(double value);
double pop();
void print_top();
void duplicate_top();
void swap_top_two();
void clear_stack();
int getop(char s[]);
int getch();
void ungetch(int c);
void ungets(char s[]);

double stack[MAXSTACK]; 
int stackPos = 0;       
char buf[MAXBUF];       
int bufPos = 0;         

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

void print_top() {
    if (stackPos > 0) {
        printf("Top of stack: %g\n", stack[stackPos - 1]);
    } else {
        printf("Error: Stack empty\n");
    }
}

void duplicate_top() {
    if (stackPos > 0) {
        push(stack[stackPos - 1]);
    } else {
        printf("Error: Stack empty\n");
    }
}

void swap_top_two() {
    if (stackPos > 1) {
        double temp = stack[stackPos - 1];
        stack[stackPos - 1] = stack[stackPos - 2];
        stack[stackPos - 2] = temp;
    } else {
        printf("Error: Not enough elements to swap\n");
    }
}

void clear_stack() {
    stackPos = 0;
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
        if (!isdigit(next) && next != '.') {
            ungetch(next);
            return '-';
        }
        c = next;
    }

    if (isdigit(c)) { 
        while (isdigit(s[++i] = c = getch()));
    }

    if (c == '.') {  
        while (isdigit(s[++i] = c = getch()));
    }

    s[i] = '\0';
    if (c != EOF) {
        ungetch(c);
    }

    return NUMBER;
}

int getch() {
    return (bufPos > 0) ? buf[--bufPos] : getchar();
}
void ungetch(int c) {
    if (bufPos >= MAXBUF) {
        printf("Error: Too many characters pushed back\n");
    } else {
        buf[bufPos++] = c;
    }
}

void ungets(char s[]) {
    int len = strlen(s);
    while (len > 0) {
        ungetch(s[--len]);
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
            case 'p': 
                print_top();
                break;
            case 'd': 
                duplicate_top();
                break;
            case 's': 
                swap_top_two();
                break;
            case 'c': 
                clear_stack();
                break;
            case 'u': 
                ungets("12345");
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
/* 5 4 + p d s u
Top of stack: 9
Result: 12345
5 20 s / d + p u c 
Top of stack: 8
Error: Stack empty
Result: 0
5 20 s / d + p c u 
Top of stack: 8
Result: 12345*/
