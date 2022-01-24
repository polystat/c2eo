#include <stdio.h>

unsigned long long a = -5;
unsigned long long *b = &a;

int main() {
    printf("%u\n", *b); // 4294967291
    *b;
    return 0;
}
