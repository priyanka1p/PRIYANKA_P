/*Extend atof to handle scientific notation of the form
123.45e-6
where a floating-point number may be followed by e or E and an optionally signed exponent.*/
#include <stdio.h>
#include <ctype.h>
#include <math.h>

double atof_extended(char s[]) {
    int i = 0;
    double num = 0.0, fraction = 1.0;
    int sign = 1, exp_sign = 1, exponent = 0;

    // Skip leading whitespaces
    while (isspace(s[i])) {
        i++;
    }
    if (s[i] == '-') {
        sign = -1;
        i++;
    } else if (s[i] == '+') {
        i++;
    }
    while (isdigit(s[i])) {
        num = 10.0 * num + (s[i] - '0');
        i++;
    }
    if (s[i] == '.') {
        i++;
        while (isdigit(s[i])) {
            num = 10.0 * num + (s[i] - '0');
            fraction *= 10.0;
            i++;
        }
    }
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        if (s[i] == '-') {
            exp_sign = -1;
            i++;
        } else if (s[i] == '+') {
            i++;
        }

        while (isdigit(s[i])) {
            exponent = 10 * exponent + (s[i] - '0');
            i++;
        }
    }
   return sign * (num / fraction) * pow(10, exp_sign * exponent);
}int getlinen(char line[], int limit) {
    int c, i = 0;

    while (i < limit - 1 && (c = getchar()) != EOF && c != '\n') {
        line[i++] = c;
    }

    if (c == '\n') {
        line[i++] = c;
    }

    line[i] = '\0';
    return i;
}

int main() {
    char line[100];

    printf("Enter num\n");

    while (getlinen(line, 100) > 0) {
        printf("value: %.10lf\n", atof_extended(line));
    }

    return 0;
}

/*Enter num
5.0           
value: 5.0000000000
6.5e9
value: 6500000000.0000000000
7.4e-20
value: 0.0000000000
6.e45
value: 5999999999999998944718434033098606571232952320.0000000000
*/
