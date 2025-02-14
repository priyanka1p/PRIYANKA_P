//Revise the main routine of the longest-line program so it will correctly print the
//length of arbitrary long input lines, and as much as possible of the text.
#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 1000 
int get_line(char line[], int max_length) {
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        if (i < max_length - 1) {
            line[i] = c; 
        }
        i++;
    }
    if (i < max_length - 1) {
        line[i] = '\0';  
    } else {
        line[max_length - 1] = '\0'; 
    }
    return i; }
int main() {
    int len, max_len = 0; 
    char line[MAX_LINE];  
    char *longest = NULL; 
    while ((len = get_line(line, MAX_LINE)) > 0) {
        printf("Length: %d | Text: %s\n", len, line);
        if (len > max_len) {  
            max_len = len;
            free(longest);  
            longest = malloc(len + 1);  
            if (longest != NULL) {
                for (int i = 0; i < len && i < MAX_LINE - 1; i++) {
                    longest[i] = line[i];
                }
                longest[len] = '\0';
            }   }
    }
    if (max_len > 0 && longest != NULL) {
        printf("\nLongest line length: %d\n", max_len);
        printf("Longest line text: %s\n", longest);
        free(longest);  
    }
    return 0;
}
/*there are many languages in india
Length: 33 | Text: there are many languages in india
Also this land has diverse cultures heritage and environment
Length: 60 | Text: Also this land has diverse cultures heritage and environment
people of india follow, unity in diversity
Length: 42 | Text: people of india follow, unity in diversity

Longest line length: 60
Longest line text: Also this land has diverse cultures heritage and environmen*/
