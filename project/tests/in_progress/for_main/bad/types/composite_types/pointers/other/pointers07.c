//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 255;
long long e = 255;
long long *pc = &c;
long long **ppc;

int main() {
    ppc = &pc;
    d = (*pc)++;
    e = **ppc;

    printf("c = %lld\n", c);
    printf("d = %lld\n", d);
    printf("e = %lld\n", e);
    printf("*pc = %lld\n", *pc);

    return 0;
}
