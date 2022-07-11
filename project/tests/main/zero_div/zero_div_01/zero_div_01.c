#include <stdio.h>

long long c = 10;
long long x = 5;

int main() {
    printf("%lld\n", x);

    x = c / x;

    printf("%lld\n", c);
    printf("%lld\n", x);

    return 0;
}
