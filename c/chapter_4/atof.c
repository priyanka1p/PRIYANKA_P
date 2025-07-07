//write custom function for atof()
#include <stdio.h>
#include <ctype.h>

double atosf(char s[]) {
    int i = 0;
    double num = 0.0;
    
    while (isspace(s[i])) {
        i++;
    }

    int sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '-' || s[i] == '+') {
        i++;
    }

    while (s[i] != '\0') {
        if (isdigit(s[i])) {
            num = 10.0 * num + (s[i] - '0');
            i++;
        }
        if (s[i] == '.') {
            i++;
            break;
        }
    }

    double fraction = 1.0;
    while (s[i] != '\0' && isdigit(s[i])) {
        num = 10.0 * num + (s[i] - '0');
        fraction *= 10.0;
        i++;
    }

    return sign * (num / fraction);
}

int getlinen(char line[], int limit) { 
    int c, i = 0;
    
    while (i < limit - 1 && (c = getchar()) != EOF && c != '\n') {
        line[i++] = c;
    }
    line[i] = '\0'; 
    return i;
}

int main() {
    char line[100];

    while (getlinen(line, 100) > 0) {
        printf("%.3lf\n", atosf(line));
    }

    return 0;
}
