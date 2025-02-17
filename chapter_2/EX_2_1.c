/*Write a program to determine the ranges of char, short, int, and long
variables, both signed and unsigned, by printing appropriate values from standard headers
and by direct computation. Harder if you compute them: determine the ranges of the various
floating-point types.*/
#include <stdio.h>

#include <limits.h>
#include <float.h>


int main() {
    printf("char:\n  Signed: %d to %d\n  Unsigned: 0 to %u\n", CHAR_MIN, CHAR_MAX, UCHAR_MAX);
    printf("short:\n  Signed: %d to %d\n  Unsigned: 0 to %u\n", SHRT_MIN, SHRT_MAX, USHRT_MAX);
    printf("int:\n  Signed: %d to %d\n  Unsigned: 0 to %u\n", INT_MIN, INT_MAX, UINT_MAX);
    printf("long:\n  Signed: %ld to %ld\n  Unsigned: 0 to %lu\n", LONG_MIN, LONG_MAX, ULONG_MAX);
    printf("long long:\n  Signed: %lld to %lld\n  Unsigned: 0 to %llu\n", LLONG_MIN, LLONG_MAX, ULLONG_MAX);

    printf("float: %e to %e\n", FLT_MIN, FLT_MAX);
    printf("double: %e to %e\n", DBL_MIN, DBL_MAX);
    printf("long double: %Le to %Le\n", LDBL_MIN, LDBL_MAX);
    

    return 0;
}
//By using predefined macro
/*char:
  Signed: -128 to 127
  Unsigned: 0 to 255
short:
  Signed: -32768 to 32767
  Unsigned: 0 to 65535
int:
  Signed: -2147483648 to 2147483647
  Unsigned: 0 to 4294967295
long:
  Signed: -9223372036854775808 to 9223372036854775807
  Unsigned: 0 to 18446744073709551615
long long:
  Signed: -9223372036854775808 to 9223372036854775807
  Unsigned: 0 to 18446744073709551615
float: 1.175494e-38 to 3.402823e+38
double: 2.225074e-308 to 1.797693e+308
long double: 3.362103e-4932 to 1.189731e+4932
*/
//by direct computation
#include <stdio.h>

void compute_signed_range(int bits, long *min, long *max) {
    *max = (1L << (bits - 1)) - 1;  
    *min = -(*max) - 1;             
}


unsigned long compute_unsigned_max(int bits) {
    return (1UL << bits) - 1;  
}

int main() {
    
    long min, max;

    compute_signed_range(8, &min, &max);
    printf("%d,char:\n  Signed: %ld to %ld\n Unsigned: 0 to %lu\n", sizeof(int),min, max, compute_unsigned_max(8));

    compute_signed_range(16, &min, &max);
    printf("short:\n  Signed: %ld to %ld\n Unsigned: 0 to %lu\n", min, max, compute_unsigned_max(16));

    compute_signed_range(32, &min, &max);
    printf("int:\n Signed: %ld to %ld\n Unsigned: 0 to %lu\n", min, max, compute_unsigned_max(32));

    compute_signed_range(64, &min, &max);
    printf("long long:\n Signed: %ld to %ld\n Unsigned: 0 to %lu\n", min, max, compute_unsigned_max(64));

    return 0;
}

