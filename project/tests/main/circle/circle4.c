#include <stdio.h>

double pi = 3.14192;
static double r = 10.0;
double p = 0.0;
double s = 0.0;
double tmp = 0.0;
double two = 2.0;

main() {
    tmp = pi * r;
    p = two * tmp;
     printf("%e\n", p);
    s = tmp * r;
    printf("%e\n", s);
}
