#include <stdio.h>

int main() {
    double a = 5.55;
    printf("%1.2f\n", *&a);
    return 0;
}
