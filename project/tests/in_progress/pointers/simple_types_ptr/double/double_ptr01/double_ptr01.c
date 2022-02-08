#include <stdio.h>

double a = 5.55;

int main() {
    printf("%1.2f\n", *&a);
    *&a;
    return 0;
}