/* How would you test the word count program? What kinds of input are most
likely to uncover bugs if there are any? */
#include <stdio.h>
#include <ctype.h>

int main() {
    int c, in_word = 0;
    int words = 0, lines = 0, characters = 0;
    while ((c = getchar()) != EOF) {
        

        if (isspace(c)) {
            in_word = 0; 
        } else if (!in_word) {
            in_word = 1;
            words++; 
        }
    }
    printf("\nWords: %d", words);

    return 0;
}

/* I'm a 	sw engineer.
\n
i like to play video games
Words: 11*/


Kinds of inputs:
1 Hello world
2 Hello      world    This	is	a	cases.
3 Hello
world

This
is
a
cases.
4  
5  self-driving cars are the future. That's gonna be amazing.
