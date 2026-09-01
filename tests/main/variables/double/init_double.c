#include <stdio.h>

double d1;
double d2 = 3.14;
double d3 = 0;

int main() {
    d1 = d2 + d3;
    printf("d1 = %f\n", d1);
    printf("d2 = %f\n", d2);
    printf("d3 = %f\n", d3);

    return 0;
}
