#include <stdio.h>

double a = 5.55;

int main() {
    printf("%f\n", *&a);
    return 0;
}
