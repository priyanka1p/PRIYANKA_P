//strncmp
int main() {
    int cmp = my_strncmp("HellWorld", "HelloMars", 5);
    printf("strncmp result: %d\n", cmp); 
    return 0;
}int my_strncmp(const char *s, const char *t, size_t n) {
    if (!s || !t) return -1; 
    while (n > 0 && *s && (*s == *t)) { 
        s++;
        t++;
        n--;
    }
    if (n == 0) return 0; 
    return (unsigned char)*s - (unsigned char)*t;
}
//strncmp result -24
