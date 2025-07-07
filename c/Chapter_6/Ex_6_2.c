/*Write a program that reads a C program and prints in alphabetical order each
group of variable names that are identical in the first 6 characters, but different somewhere
thereafter. Don't count words within strings and comments. Make 6 a parameter that can be set
from the command line.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define DEFAULT_PREFIX_LEN 3


struct vlist {
    char *varname;
    struct vlist *next;
};

struct gnode {
    char *prefix;
    struct vlist *vars;
    struct gnode *left;
    struct gnode *right;
};

void treeprint(struct gnode *);
struct gnode *addgroup(struct gnode *, char *, char *);
struct gnode *group_root = NULL;
int getword(char *, int);
int iskeyword(char *);
int prefix_length = DEFAULT_PREFIX_LEN; 

int main(int argc, char *argv[]) {
    struct tnode *root = NULL;
    char word[MAXWORD];

    if (argc > 1)
        prefix_length = atoi(argv[1]);

    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0]) && !iskeyword(word))
            group_root = addgroup(group_root, word, word);
    }

    treeprint(group_root);
    return 0;
}

struct gnode *addgroup(struct gnode *p, char *prefix, char *word) {
    int cond;
    char truncated[MAXWORD];
    strncpy(truncated, prefix, prefix_length);
    truncated[prefix_length] = '\0';

    if (p == NULL) {  
        p = (struct gnode *) malloc(sizeof(struct gnode));
        p->prefix = strdup(truncated);
        p->vars = (struct vlist *) malloc(sizeof(struct vlist));
        p->vars->varname = strdup(word);
        p->vars->next = NULL;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(truncated, p->prefix)) == 0) { 
        struct vlist *newvar = (struct vlist *) malloc(sizeof(struct vlist));
        newvar->varname = strdup(word);
        newvar->next = p->vars;
        p->vars = newvar;
    } else if (cond < 0)  
        p->left = addgroup(p->left, truncated, word);
    else  
        p->right = addgroup(p->right, truncated, word);

    return p;
}

void treeprint(struct gnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("Group [%s]:\n", p->prefix);
        struct vlist *var = p->vars;
        while (var) {
            printf("  %s\n", var->varname);
            var = var->next;
        }
        treeprint(p->right);
    }
}

int iskeyword(char *word) {
    static char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register",  "return",
        "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
        "union", "unsigned", "void", "volatile", "while"
    };
    int n = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < n; i++)
        if (strcmp(word, keywords[i]) == 0)
            return 1;

    return 0;
}

int getword(char *word, int lim) {
    int c;
    char *w = word;

    while (isspace(c = getchar()))
        ;

    if (c == '/') {
        if ((c = getchar()) == '*') { 
            while ((c = getchar()) != EOF)
                if (c == '*' && (c = getchar()) == '/')
                    break;
        } else if (c == '/') { 
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
    } 
    if (c == '"' || c == '\'') {
        int quote = c;
        while ((c = getchar()) != EOF && c != quote)
            if (c == '\\')
                getchar(); 
    }
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
/*priyanka
periyar
prinku
banana
banner
bangalore
banker
beggar
giraffer
Group [ban]:
  banker
  bangalore
  banner
  banana
Group [beg]:
  beggar
Group [gir]:
  giraffer
Group [per]:
  periyar
Group [pri]:
  prinku
  priyanka
*/
