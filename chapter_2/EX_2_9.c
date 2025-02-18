//In a two's complement number system, x &= (x-1) deletes the rightmost 1-bit
//in x. Explain why. Use this observation to write a faster version of bitcount.
#include <stdio.h>
int bitcount(int x) {
    int count = 0;
    while (x) {
        x &= (x - 1); 
        count++;
    }
    return count;
}

int main() {
    int num = 29; 
    printf("Number of 1 bits in %d: %d\n", num, bitcount(num));
    return 0;
}

//Number of 1 bits in 29: 4
