/*Write the function strindex(s,t) which returns the position of the rightmost
occurrence of t in s, or -1 if there is none.*/
#include<stdio.h>
#include<string.h>
int strindex(char a[] , char b){
    int n;
    n= strlen(a);
    for(int j=n-1;j>=0;j--){
        if(b == a[j]){
            return j;
        }
    }
    return -1;
}


int main(){
    char s[]="hello worldo";
    char t='o';
    int n;
    n=strindex(s,t);
    if(n==-1){
        printf("There is no occurence");
    }
    else{
        printf("%d",n);
    }
    return 0;
}
//11
