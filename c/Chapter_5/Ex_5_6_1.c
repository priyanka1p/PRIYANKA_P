//getline
#include <stdio.h>
int getline_ptr(char *s, int lim) {
    char *p = s;
    int c;
    
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        *p++ = c;
    
    if (c == '\n')
        *p++ = c;
    
    *p = '\0';
    return p - s;
}

int main() {
    char line[100];
    printf("Enter a line: ");
    int len = getline_ptr(line, 100);
    printf("You entered (%d characters): %s", len, line);
    return 0;
}
/*Enter a line: i am priyanka
You entered (14 characters): i am priyanka*/
