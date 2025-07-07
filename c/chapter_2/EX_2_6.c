//Write a function setbits(x,p,n,y) that returns x with the n bits that begin at
//position p set to the rightmost n bits of y, leaving the other bits unchanged.
#include <stdio.h>
int setbits(int x, int p, int n,  int y) {
    int mask = ((1 << n) - 1) << (p - n + 1);
    int y_bits = (y & ((1 << n) - 1)) << (p - n + 1); 
    
    return (x & ~mask) | y_bits;
}

int main() {
    int x = 204;
    int y = 170; 
    int p = 5, n = 3;

    int result = setbits(x, p, n, y);
    
    printf("Result: %d", result);
    
    return 0;
}
//result:212
