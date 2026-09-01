//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

extern long long c;

int main() {
    c = 25L;
    printf("%lld", c);
    return 0;
}

long long c;

void f() {
    c = 10L;
    printf("%lld", c);
}
