/*Write the function strend(s,t), which returns 1 if the string t occurs at the
end of the string s, and zero otherwise.*/
#include <stdio.h>

int strend(const char *s, const char *t);

int main() {
    char s1[] = "Hello, World!";
    char t1[] = "World!";
    char t2[] = "Hello";
    
    printf("t1: %d\n", strend(s1, t1)); 
    printf("t2: %d",  strend(s1, t2));
    
    return 0;
}
int strend(const char *s, const char *t) {
    const char *s_edge = s;
    const char *t_edge = t;

    while (*s)
        s++;
    while (*t)
        t++;

    while (t >= t_edge && *s == *t) {
        s--;
        t--;
    }

    return (t < t_edge) ? 1 : 0;
}
/*t1: 1
t2: 0*/
