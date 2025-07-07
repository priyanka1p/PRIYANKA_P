//Write a function rightrot(x,n) that returns the value of the integer x rotated
//to the right by n positions.#include <stdio.h>

int rightrot(int x, int n) {
    int word_size = sizeof(x) * 8; 
    n = n % word_size;
    return (x >> n) | (x << (word_size - n));
}
void printBinary(unsigned int num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 4 == 0) printf(" ");
    }
    printf("\n");
}

int main() {
    int x = 170;
    int n = 3;

    int result = rightrot(x, n);

    printf("After rotating %d positions to the right: ", n);
    printBinary(result);

    return 0;
}
//After rotating 3 positions to the right: 0100 0000 0000 0000 0000 0000 0001 0101
