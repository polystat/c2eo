#include <stdio.h>

int main() {
    int a = 5;
    int b = 5;
    int *c = &b;
    printf("%d\n", *c);
    printf("%d\n", *&a);
    return 0;
}
