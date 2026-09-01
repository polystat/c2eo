#include <stdio.h>

int main() {
    double a = 5.55;
    double b = 12.55;
    double *c = &b;
    printf("%1.2f\n", *c);
    printf("%1.2f\n", *&a);
    return 0;
}
