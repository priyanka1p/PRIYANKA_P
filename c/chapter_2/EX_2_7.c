/*Write a function invert(x,p,n) that returns x with the n bits that begin at
position p inverted (i.e., 1 changed into 0 and vice versa), leaving the others unchanged.*/
#include <stdio.h>
int invert(int x, int p, int n) {
    int mask = ((1 << n) - 1) << (p - n + 1);
    return x ^ mask;
}
void printBinary(int num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 4 == 0) printf(" ");
    }
    printf("\n");
}

int main() {
    int x = 170;
    int p = 5, n = 3;

    printf("Original x: ");
    printBinary(x);

    int result = invert(x, p, n);

    printf("After inverting %d bits at position %d: ", n, p);
    printBinary(result);

    return 0;
}

//Original x: 0000 0000 0000 0000 0000 0000 1010 1010 
//After inverting 3 bits at position 5: 0000 0000 0000 0000 0000 0000 1001 0010 
  
