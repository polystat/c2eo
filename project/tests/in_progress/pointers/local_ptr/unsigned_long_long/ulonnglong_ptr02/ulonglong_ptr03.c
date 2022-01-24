#include <stdio.h>

int main() {
    unsigned long long a = -5; // 4294967291
    unsigned long long *b = &a;
    printf("%u\n", *b);
    *b;
    return 0;
}
