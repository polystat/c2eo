#include <stdio.h>

unsigned int a = -5;
unsigned int *b = &a;

int main() {
    printf("%u\n", *b); // 4294967291
    return 0;
}
