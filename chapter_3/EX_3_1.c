/*Our binary search makes two tests inside the loop, when one would suffice (at
the price of more tests outside.) Write a version with only one test inside the loop and measure
the difference in run-time.*/
#include <stdio.h>
int binsearch(int x, int v[], int n) {
    int low = 0, high = n - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x < v[mid])
            high = mid - 1;
        else if (x > v[mid])
            low = mid + 1;
        else
            return mid; 
    }
    return -1;
}
int binsearch_one_test(int x, int v[], int n) {
    int low = 0, high = n - 1, mid;
    while (low < high) { 
        mid = (low + high) / 2;
        if (x > v[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return (v[low] == x) ? low : -1;
}
int main() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19}; 
    int n = sizeof(arr) / sizeof(arr[0]);
    int key = 11;

    int index1 = binsearch(key, arr, n);
    int index2 = binsearch_one_test(key, arr, n);

    printf("2 test Binary Search: Found at index %d\n", index1);
    printf("1 test Binary Search: Found at index %d\n", index2);

    return 0;
}
//2 test Binary Search: Found at index 5
//1 test Binary Search: Found at index 5
