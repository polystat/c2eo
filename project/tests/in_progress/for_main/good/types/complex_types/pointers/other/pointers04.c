//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 10;
long long *pc;

int main() {
    pc = &c;
    d = *pc;
    printf("c = %lld\n", c);
    printf("d = %lld\n", d);
    printf("*pc = %lld\n", *pc);

    return 0;
}
