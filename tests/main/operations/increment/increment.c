#include <stdio.h>

typedef long long i64;

i64 x = 42;
i64 y = 0;

int main() {
    x = 1 + x;
    printf("%ld\n", x);
    x = x + 1;
    printf("%ld\n", x);
    return 0;
}