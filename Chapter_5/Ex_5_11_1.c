//entab
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT_TABSIZE 8 
#define MAX_TABS 100       

int tabstops[MAX_TABS];  

void set_default_tabs();
void parse_tabstops(int argc, char *argv[]);
int next_tabstop(int col);

int main(int argc, char *argv[]) {
    int c, col = 0, space_count = 0;

    parse_tabstops(argc, argv);

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            space_count++;
            if (col + space_count == next_tabstop(col)) {
                putchar('\t');
                space_count = 0;
                col = next_tabstop(col);
            }
        } else {
            while (space_count > 0) {
                putchar(' ');
                space_count--;
                col++;
            }
            putchar(c);
            col = (c == '\n') ? 0 : col + 1;
        }
    }
    return 0;
}

void set_default_tabs() {
    for (int i = 0; i < MAX_TABS; i++)
        tabstops[i] = (i + 1) * DEFAULT_TABSIZE;
}

void parse_tabstops(int argc, char *argv[]) {
    if (argc == 1) {
        set_default_tabs();
        return;
    }
    for (int i = 1; i < argc && i - 1 < MAX_TABS; i++) {
        tabstops[i - 1] = atoi(argv[i]);
        if (tabstops[i - 1] <= 0) {
            printf("Invalid tab stop: %s\n", argv[i]);
            exit(1);
        }
    }
}

int next_tabstop(int col) {
    for (int i = 0; i < MAX_TABS; i++) {
        if (tabstops[i] > col)
            return tabstops[i];
    }
    return col + DEFAULT_TABSIZE;  
}
