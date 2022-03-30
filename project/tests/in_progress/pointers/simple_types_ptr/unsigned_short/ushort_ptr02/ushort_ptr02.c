#include <stdio.h>

unsigned short a = 5;
unsigned short *b = &a;

int main() {
    printf("%u\n", *b);
    *b;
    return 0;
}
