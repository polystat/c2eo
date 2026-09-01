#include <stdio.h>

long long c = 10;

long long func2(long long x);

long long func1(long long x) {
    return func2(x);
}

long long func2(long long x) {
    return x;
}

long long func3(long long x) {
    return func2(x);
}

int main() {
    long long a;
    a = func1(c);
    printf("%lld\n", a);

    long long b;
    b = func1(a);
    printf("%lld\n", b);

    return 0;
}
