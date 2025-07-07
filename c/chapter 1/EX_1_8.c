//Write a program to count blanks, tabs, and newlines.
#include <stdio.h>

int main() {
    int c;
    int blanks = 0, tabs = 0, newlines = 0;

    printf("Enter text:\n");

    while ((c = getchar()) != EOF) {
        if (c == ' ')
            blanks++;
        else if (c == '\t')
            tabs++;
        else if (c == '\n')
            newlines++;
    }

    printf("\nCounts:\n");
    printf("Blanks: %d\n", blanks);
    printf("Tabs: %d\n", tabs);
    printf("Newlines: %d\n", newlines);

    return 0;
}
/*Enter Text:
Priyanka	Prabakaran
I am from villupuram
Counts:
Blanks: 3
Tabs: 1
Newlines: 1*/
