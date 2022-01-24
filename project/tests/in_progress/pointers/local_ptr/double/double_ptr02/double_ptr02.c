#include <stdio.h>

int main() {
    double a = 5.55;
    double *b = &a;
    printf("%1.2f\n", *b);
    *b;
    return 0;
}
