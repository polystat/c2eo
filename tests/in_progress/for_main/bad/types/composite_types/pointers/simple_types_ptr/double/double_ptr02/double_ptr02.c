#include <stdio.h>

double a = 5.55;
double *b = &a;

int main() {
    printf("%f\n", *b);
    return 0;
}
