#include <stdio.h>
long long b = 10;
long long c = 30;

int main() {
    b = b + c;
    printf("%lld\n", b);
    b;
    c = b - c;
    printf("%lld\n", c);
    c;
    b = b - c;
    printf("%lld\n", b);
    b;
    return 0;
}
