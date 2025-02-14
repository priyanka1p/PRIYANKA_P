/*Write a program to copy its input to its output, replacing each tab by \t, each
backspace by \b, and each backslash by \\. This makes tabs and backspaces visible in an
unambiguous way.
  */
  #include <stdio.h>

int main()
{
    int c;
    while((c=getchar())!=EOF){
        if(c=='\t'){
            printf("\\t");
        }
        else if(c=='\b'){
            printf("\\b");
        }
        else if(c=='\\'){
            printf("\\\\");
        }
        else
            putchar(c);
    }
    printf("Hello World");

    return 0;
}
/* hi 	Priyanka
hi \tPriyanka
d\O\B: 01.05.2004
d\\O\\B: 01.05.2004
Hello World  */
