//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long x = 5;

int main() {
    printf("%lld\n", x);
    x;

    x = c / x;

    printf("%lld\n", c);
    c;
    printf("%lld\n", x);
    x;

    return 0;
}
