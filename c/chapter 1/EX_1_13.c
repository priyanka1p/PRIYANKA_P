/*Write a program to print a histogram of the lengths of words in its input. It is
easy to draw the histogram with the bars horizontal; a vertical orientation is more challenging.
HORIZONTAL HISTOGRAM */
#include<stdio.h>
#include<ctype.h>
#define MAX_WORD_LENGTH 20
int main(){
    int c,length=0;
    int arr[MAX_WORD_LENGTH]={0};
    while((c=getchar()) != EOF){
        if(isspace(c)){
            
            if(length<MAX_WORD_LENGTH){
                arr[length]++;
                length=0;
               
            }
            else{
                arr[MAX_WORD_LENGTH-1]++;
                length=0;
            }
            }
        
        else{
            length++;
        }
    }
    if (length<MAX_WORD_LENGTH-1)
        arr[length]++;
    else
        arr[MAX_WORD_LENGTH-1]++;
        
    for (int i = 1; i < MAX_WORD_LENGTH; i++) {
        if (arr[i] > 0) {
            printf("\n %d: ", i);
            for (int j = 0; j < arr[i]; j++) {
                printf("*");
            }
            
        }
    }
}/*I stay in villupuram with my family and friends. 
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
 1: *
 2: **
 3: *
 4: **
 6: *
 8: *
 10: *
 19: *
*/
//VERTICAL HISTOGRAM
#include <stdio.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 20  

int main() {
    int c, length = 0;
    int word_lengths[MAX_WORD_LENGTH] = {0};

    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            if (length > 0) {
                if (length < MAX_WORD_LENGTH) {
                    word_lengths[length]++;
                } else {
                    word_lengths[MAX_WORD_LENGTH - 1]++;
                }
                length = 0;
            }
        } else {
            length++;
        }
    if (length < MAX_WORD_LENGTH) {
        word_lengths[length]++;
        }
    else {
        word_lengths[MAX_WORD_LENGTH - 1]++;
        }

    int max_count = 0;
    for (int i = 1; i < MAX_WORD_LENGTH; i++) {
        if (word_lengths[i] > max_count) {
            max_count = word_lengths[i];
        }
    }
for (int i = max_count; i > 0; i--) {
        for (int j = 1; j < MAX_WORD_LENGTH; j++) {
            if (word_lengths[j] >= i) {
                printf("* ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    for (int i = 1; i < MAX_WORD_LENGTH; i++) {
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}

/*MS Dhoni is always a calm and composed leader.
  *                                   
* * * * * * * *                       
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 */
