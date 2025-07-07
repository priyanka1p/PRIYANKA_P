/*Write a function escape(s,t) that converts characters like newline and tab into
visible escape sequences like \n and \t as it copies the string t to s. Use a switch. Write a
function for the other direction as well, converting escape sequences into the real characters.#include <stdio.h>*/
void escape(char s[], const char t[]) {
    int i, j;
    for (i = 0, j = 0; t[i] != '\0'; i++) {
        switch (t[i]) {
            case '\n': s[j++] = '\\'; s[j++] = 'n'; break;
            case '\t': s[j++] = '\\'; s[j++] = 't'; break;
            case '\\': s[j++] = '\\'; s[j++] = '\\'; break;
            default:   s[j++] = t[i]; break;
        }
    }
    s[j] = '\0';
}
void unescape(char s[], const char t[]) {
    int i, j;
    for (i = 0, j = 0; t[i] != '\0'; i++) {
        if (t[i] == '\\' && t[i+1] != '\0') {
            switch (t[i+1]) {
                case 'n': s[j++] = '\n'; i++; break;
                case 't': s[j++] = '\t'; i++; break;
                case '\\': s[j++] = '\\'; i++; break;
                default: s[j++] = '\\'; break;
            }
        } else {
            s[j++] = t[i];
        }
    }
    s[j] = '\0';
}
int main() {
    char original[] = "Hello\tWorld\nThis is \\C!";
    char escaped[100], unescaped[100];
    printf("Original sequence: %s\n",original);
    escape(escaped, original);
    printf("Escaped: %s\n", escaped);
    unescape(unescaped, escaped);
    printf("Unescaped: %s\n", unescaped);
    return 0;
}
/*Original sequence: Hello	World
This is \C!
Escaped: Hello\tWorld\nThis is \\C!
Unescaped: Hello	World
This is \C!
*/
