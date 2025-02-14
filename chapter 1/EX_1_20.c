/*Write a program detab that replaces tabs in the input with the proper number
of blanks to space to the next tab stop. Assume a fixed set of tab stops, say every n columns.
Should n be a variable or a symbolic parameter*/
#include <stdio.h>
#define TAB_WIDTH
void detab(int tab_width) {
    int c, pos = 0; 
    while ((c = getchar()) != EOF) {
        if (c == '\t') {
            int spaces = tab_width - (pos % tab_width);
            for (int i = 0; i < spaces; i++) {
                putchar(' ');
                pos++;
            }
        } else {
            putchar(c);
            pos++;
            if (c == '\n') {
                pos = 0;
            }
        }
    }
}
int main() {
    int user_tab_width;
    printf("Enter tab width: ");
    scanf("%d", &user_tab_width);
    detab(user_tab_width);
    return 0;
}

/*Enter tab width: 4

hello   	world!!		
hello       world!!
*/
