//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

//long long x;
long long x = 6;

int main() {
    x = 42 / x;
    printf("%lld\n", x);
}
