#include <stdio.h>

unsigned short a = -5;
unsigned short *b = &a;

int main() {
    printf("%u\n", *b); // 65531
    *b;
    return 0;
}