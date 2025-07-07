//strcpy
char *my_strncpy(char *s, const char *t, size_t n) {
    if (!s || !t) return NULL;
    char *start = s;
    while (n > 0 && (*s = *t)) {
        s++;
        t++;
        n--;
    }
    while (n > 0) {
        *s++ = '\0';
        n--;
    }
    return start;
}
