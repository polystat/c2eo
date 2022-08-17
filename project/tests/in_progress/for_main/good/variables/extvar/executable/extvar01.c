//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

extern long long c;

int main() {
    c = 5L;
    printf("%lld", c);
    return 0;
}

long long c;
