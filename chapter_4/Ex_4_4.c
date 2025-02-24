/*Add the commands to print the top elements of the stack without popping, to
duplicate it, and to swap the top two elements. Add a command to clear the stack.*/
#include <stdio.h>
#include <stdlib.h>  
#include <ctype.h> 
#include <math.h>   

#define MAXSTACK 100  
#define MAXBUF 100   
#define NUMBER '0' 

void push(double value);
double pop();
void printTop();   
void duplicate();  
void swap();       
void clearStack(); 

int getop(char s[]);
int getch();
void ungetch(int c);

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

void printTop() {
    if (stackPos > 0) {
        printf("Top of Stack: %.8g\n", stack[stackPos - 1]);
    } else {
        printf("Error: Stack empty\n");
    }
}

void duplicate() {
    if (stackPos > 0) {
        double top = stack[stackPos - 1];
        push(top);
    } else {
        printf("Error: Stack empty\n");
    }
}

void swap() {
    if (stackPos > 1) {
        double first = stack[stackPos - 1];
        double second = stack[stackPos - 2];
        stack[stackPos - 1] = second;
        stack[stackPos - 2] = first;
    } else {
        printf("Error: Not enough elements to swap\n");
    }
}

// Clear the stack
void clearStack() {
    stackPos = 0;
    printf("Stack cleared\n");
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
            case 't':  
                printTop();
                break;
            case 'd':  
                duplicate();
                break;
            case 's':  
                swap();
                break;
            case 'c':  
                clearStack();
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
/*3 4 5 t - d - 4 + s %
Top of Stack: 5
Result: 1
6.3 4.5 s -
Result: -1.8
6.3 9.3 s -
Result: 3
Result: 0
6.3 t
Top of Stack: 6.3
Result: 6.3
Result: 0
6.3 3.3 3.6 t - d + - 
Top of Stack: 3.6
Result: 6.9
4.5 3.4 + c 0 -
Stack cleared
Error: Stack empty
Result: 0*/
