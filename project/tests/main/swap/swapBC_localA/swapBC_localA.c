#include <stdio.h>
long long b = 10;
long long c = 30;

int main() {
    long long a = 0;
    a = b;
    b = c;
    c = a;
    a;
    b;
    c;
    printf("%lld\n", a);
    printf("%lld\n", b);
    printf("%lld\n", c);
}
