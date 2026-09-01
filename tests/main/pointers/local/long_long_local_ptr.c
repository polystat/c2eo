#include <stdio.h>

int main() {
    long long a = 5;
    long long b = 8;
    long long *c = &b;
    printf("%lld\n", *c);
    printf("%lld\n", *&a);
    return 0;
}

