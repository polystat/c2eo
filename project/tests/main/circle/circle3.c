#include <stdio.h>

double pi = 3.14192;
static double r = 10.0;
double p = 0.0;
double s = 0.0;
double two = 2.0;
main() {
    p = two * pi * r;
    printf("%f\n", p);
    s = pi * r * r;
    printf("%f\n", s);
}
