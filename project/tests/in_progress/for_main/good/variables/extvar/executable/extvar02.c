//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

extern long long c;

long long c;

int main() {
    c = 15L;
    printf("%lld", c);
    return 0;
}
