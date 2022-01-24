#include <stdio.h>

double a = 5.55;
double *b = &a;

int main() {
    printf("%1.2f\n", *b);
    *b;
    return 0;
}
