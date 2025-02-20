/*Add access to library functions like sin, exp, and pow. See <math.h> in
Appendix B, Section 4.*/
#include <stdio.h>
#include <math.h>
//pow(x,y) where x, y are double
int main() {
    double x = 2.0;
    printf("sin(%.2f) = %f\n", x, sin(x));
    printf("exp(%.2f) = %.4f\n", x, exp(x));
    printf("pow(%.2f, %f) = %.4f\n", x, 3.0, pow(x, 3.0));
    return 0;

}
/*sin(2.00) = 0.909297
exp(2.00) = 7.3891
pow(2.00, 3.000000) = 8.0000*/
