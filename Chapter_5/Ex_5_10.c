/*Write the program expr, which evaluates a reverse Polish expression from the
command line, where each operator or operand is a separate argument. For example,
expr 2 3 4 + **/
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h> 

#define MAXSTACK 100 
double stack[MAXSTACK];
int sp = 0;  
void push(double val);
double pop(void);
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: expr <RPN expression>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (isdigit(argv[i][0]) || (argv[i][0] == '-' && isdigit(argv[i][1]))) {
            push(atof(argv[i])); 
        } else if (argv[i][1] == '\0') {  
            double op2;
            switch (argv[i][0]) {
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
                    if (op2 != 0.0)
                        push(pop() / op2);
                    else {
                        printf("Error: Division by zero\n");
                        return 1;
                    }
                    break;
                case '%':
                    op2 = pop();
                    if (op2 != 0.0)
                        push((int)pop() % (int)op2);
                    else {
                        printf("Error: Modulo by zero\n");
                        return 1;
                    }
                    break;
                default:
                    printf("Error: Unknown operator %s\n", argv[i]);
                    return 1;
            }
        } else {
            printf("Error: Invalid token %s\n", argv[1]);
            return 1;
        }
    }

    printf("Result: %.2f\n", pop()); 
    return 0;
}
void push(double val) {
    if (sp < MAXSTACK)
        stack[sp++] = val;
    else
        printf("Error: Stack full\n");
}
double pop(void) {
    if (sp > 0)
        return stack[--sp];
    else {
        printf("Error: Stack empty\n");
        return 0.0;
    }
}
/*./ex_5_100.out 5 4 +
Result: 9.00
*/
