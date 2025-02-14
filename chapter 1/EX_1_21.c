/*Write a program entab that replaces strings of blanks by the minimum number
of tabs and blanks to achieve the same spacing. Use the same tab stops as for detab. When
either a tab or a single blank would suffice to reach a tab stop, which should be given
preference?*/
#include <stdio.h>
#define TAB_WIDTH 4 
void entab(int tab_width) {
    int c, space_count = 0, pos = 0;
    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            space_count++;
            pos++;
            if (pos % tab_width == 0) {
                putchar('\t');
                space_count = 0;
            }
        } else {
            while (space_count > 0) {
                putchar(' ');
                space_count--;
            }
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
    entab(user_tab_width);
    return 0;
}
/*Enter tab width: 4

hello      world
hello	   world
*/
