#include <stdio.h>

double pi = 3.14192;
static double r = 10.0;
double p = 0.0;
double s = 0.0;
main() {
    p = 2.0 * pi * r;
    printf("%e\n", p);
    p;
    s = pi * r * r;
    printf("%e\n", s);
    s;
}
