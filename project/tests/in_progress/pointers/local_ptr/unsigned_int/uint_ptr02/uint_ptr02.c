#include <stdio.h>

int main() {
    unsigned int a = 5;
    unsigned int *b = &a;
    printf("%u\n", *b);
    *b;
    return 0;
}
