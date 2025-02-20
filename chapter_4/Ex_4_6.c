/*Add commands for handling variables. (It's easy to provide twenty-six variables
with single-letter names.) Add a variable for the most recently printed value.*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXVARS 26  

double vars[MAXVARS]; 
double last_value = 0; 

double evaluate(char var, double value) {
    if (var == '_') 
        return last_value;
    if (isalpha(var)) {
        vars[var - 'a'] = value;
        return value;
    }
    return value;
}

int main() {
    char var;
    double val;
    char op;

    printf("Type 'q' to quit.\n\n");

    while (1) {
        printf("> ");
        if (scanf(" %c", &var) != 1 || var == 'q') break;

        if (var == '_') {
            printf("Last value: %.2f\n", last_value);
            continue;
        }

        if (!isalpha(var)) {
            printf("Invalid variable. Use a-z.\n");
            continue;
        }

        if (scanf(" %c", &op) != 1 || op != '=') {
            printf("Invalid syntax. Use 'var = value'.\n");
            continue;
        }

        if (scanf("%lf", &val) != 1) {
            printf("Invalid value.\n");
            continue;
        }

        last_value = evaluate(var, val);
        printf("%c = %.2f\n", var, last_value);
    }

    return 0;
}
/*Type 'q' to quit.

> a
=
5
a = 5.00
> b
=
1-
b = 1.00
> Invalid variable. Use a-z.
> _
Last value: 1.00
*/
