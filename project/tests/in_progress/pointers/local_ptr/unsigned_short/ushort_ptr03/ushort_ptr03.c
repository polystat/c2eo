#include <stdio.h>

int main() {
    unsigned short a = -5; // 65531
    unsigned short *b = &a;
    printf("%u\n", *b);
    *b;
    return 0;
}