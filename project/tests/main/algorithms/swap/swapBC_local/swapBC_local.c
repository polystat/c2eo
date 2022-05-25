#include <stdio.h>

int main() {
    long long b = 10;
    long long c = 30;
    b = b + c;
    printf("%lld\n", b);
    c = b - c;
    printf("%lld\n", c);
    b = b - c;
    printf("%lld\n", b);
    return 0;
}
