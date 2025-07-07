//strcat
int main() {
    char s1[20] = "Hello";

    char t[] = "World!";
    my_strncat(s1, t, 3);
    printf("strncat result: %s\n", s2); 
}
char *my_strncat(char *s, const char *t, size_t n) {
    if (!s || !t) return NULL; 
    char *start = s;
    while (*s)
        s++;
    while (n-- > 0 && (*s++ = *t++)) 
        ;
    *s = '\0'; 
    return start;
}
//HelloWor
