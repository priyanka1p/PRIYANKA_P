/*Functions like isupper can be implemented to save space or to save time.
Explore both possibilities.*/
#include <stdio.h>
char is_upper_lookup[256] = {0};
int isupper_time_efficient(int c) {
    return is_upper_lookup[c];
}
int isupper_space_efficient(int c) {
    return (c >= 'A' && c <= 'Z');
}

int main() {
    for (int i = 'A'; i <= 'Z'; i++)
        is_upper_lookup[i] = 1;
    printf("%d\n", isupper_time_efficient('A')); 
    printf("%d\n", isupper_time_efficient('a')); 
    printf("%d\n", isupper_space_efficient('A')); 
    return 0;
}
/*
  1
  0
  1*/
