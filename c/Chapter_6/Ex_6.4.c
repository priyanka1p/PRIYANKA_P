/*Write a program that prints the distinct words in its input sorted into decreasing
order of frequency of occurrence. Precede each word by its count.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100

struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *);
void tree_to_array(struct tnode *, struct tnode **, int *);
void sort_by_frequency(struct tnode **, int);
void treeprint(struct tnode **, int);
int getword(char *, int);

struct tnode *talloc(void) {
    return (struct tnode *) malloc(sizeof(struct tnode));
}

char *strdup_custom(char *s) {
    char *p = (char *) malloc(strlen(s) + 1);
    if (p != NULL)
        strcpy(p, s);
    return p;
}
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) { 
        p = talloc();
        p->word = strdup_custom(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++; 
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }

    return p;
}
void tree_to_array(struct tnode *p, struct tnode **arr, int *index) {
    if (p != NULL) {
        tree_to_array(p->left, arr, index);
        arr[(*index)++] = p;
        tree_to_array(p->right, arr, index);
    }
}

int compare(const void *a, const void *b) {
    struct tnode *nodeA = *(struct tnode **)a;
    struct tnode *nodeB = *(struct tnode **)b;
    return nodeB->count - nodeA->count; 
}

void sort_by_frequency(struct tnode **arr, int n) {
    qsort(arr, n, sizeof(struct tnode *), compare);
}

void treeprint(struct tnode **arr, int n) {
    for (int i = 0; i < n; i++)
        printf("%4d %s\n", arr[i]->count, arr[i]->word);
}

int getword(char *word, int lim) {
    int c;
    char *w = word;
    while (isspace(c = getchar()));

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

int main() {
    struct tnode *root = NULL;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0])) 
            root = addtree(root, word);
    struct tnode *wordArray[1000]; 
    int wordCount = 0;
    tree_to_array(root, wordArray, &wordCount);
    sort_by_frequency(wordArray, wordCount);
    treeprint(wordArray, wordCount);

    return 0;
}
/*priyanka
chopra
is
an 
internatonal
actress
priyanka 
prabakaran
is a
sofrware 
developer
   2 is
   2 priyanka
   1 a
   1 actress
   1 an
   1 chopra
   1 developer
   1 internatonal
   1 prabakaran
   1 sofrware
*/
