/*Write a cross-referencer that prints a list of all words in a document, and for
each word, a list of the line numbers on which it occurs. Remove noise words like ``the,''
``and,'' and so on.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define NOISE_WORDS_COUNT 10

struct lnode {
    int line;
    struct lnode *next;
};

struct tnode {
    char *word;
    struct lnode *lines; 
    struct tnode *left;
    struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *, int);
void treeprint(struct tnode *);
int getword(char *, int);
int is_noise_word(char *);

char *noise_words[NOISE_WORDS_COUNT] = {
    "the", "and", "is", "in", "to", "of", "it", "for", "on", "that"
};

int line_number = 1; 

int main() {
    struct tnode *root = NULL;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0]) && !is_noise_word(word));
            root = addtree(root, word, line_number);
    }

    treeprint(root);
    return 0;
}

struct tnode *addtree(struct tnode *p, char *w, int lineno) {
    int cond;

    if (p == NULL) { 
        p = (struct tnode *) malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->lines = (struct lnode *) malloc(sizeof(struct lnode));
        p->lines->line = lineno;
        p->lines->next = NULL;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        struct lnode *l = p->lines;
        while (l->next != NULL && l->line != lineno) 
            l = l->next;
        if (l->line != lineno) { 
            l->next = (struct lnode *) malloc(sizeof(struct lnode));
            l->next->line = lineno;
            l->next->next = NULL;
        }
    } else if (cond < 0) {
        p->left = addtree(p->left, w, lineno);
    } else {
        p->right = addtree(p->right, w, lineno);
    }

    return p;
}

void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%s: ", p->word);
        struct lnode *l = p->lines;
        while (l) {
            printf("%d ", l->line);
            l = l->next;
        }
        printf("\n");
        treeprint(p->right);
    }
}

int is_noise_word(char *word) {
    for (int i = 0; i < NOISE_WORDS_COUNT; i++)
        if (strcmp(word, noise_words[i]) == 0)
            return 1;
    return 0;
}

int getword(char *word, int lim) {
    int c;
    char *w = word;

    while (isspace(c = getchar()))
        if (c == '\n')
            line_number++;

    if (c != EOF)
        *w++ = c;

    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++) {
        if (!isalnum(*w = getchar()) && *w != '_') {
            ungetc(*w, stdin);
            break;
        }
    }

    *w = '\0';
    return word[0];
}
/*i am priyanka 
i am from villupuram
i have completed bachelor of engineering in ece.
.: 3 
am: 1 2 
bachelor: 3 
completed: 3 
ece: 3 
engineering: 3 
from: 2 
have: 3 
i: 1 2 3 
in: 3 
of: 3 
priyanka: 1 
villupuram: 2 */
