#include <stdio.h>

int main() {
    unsigned int a = -5; // 4294967291
    unsigned int *b = &a;
    printf("%u\n", *b);
    return 0;
}
