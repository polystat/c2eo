//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 100;

int main() {
    d = *&c;
    printf("c = %lld\n", c);
    printf("d = %lld\n", d);

    return 0;
}
