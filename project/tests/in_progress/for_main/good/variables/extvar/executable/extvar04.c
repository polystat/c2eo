//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

extern long long c;

int main() {
    c = 5L;
    printf("%lld", c);
    return 0;
}

long long c;

void f() {
    c = 10L;
    printf("%lld", c);
}

extern long long c;

void g() {
    c = 20L;
    printf("%lld", c);
}
