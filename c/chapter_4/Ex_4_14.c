//Define a macro swap(t,x,y) that interchanges two arguments of type t.
//(Block structure will help.)
#include <stdio.h>
#define SWAP(x,y) {int temp=x; x=y; y=temp;}


int main() {
    int x=5;
    int y=100;
    SWAP(x,y);
    printf("%d %d",x,y);
    return 0;
}
//100 5
