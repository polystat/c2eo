#include <stdio.h>

int main() {
    unsigned short a = 5;
    unsigned short *b = &a;
    printf("%u\n", *b);
    return 0;
}
