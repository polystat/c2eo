//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 10;
long long *pc;
long long *pd;

int main() {
    pc = & c;
    d = *&*&*&*pc;
    pd = &*&*&*pc;
    printf("c = %lld\n", c);
    printf("d = %lld\n", d);
    printf("*pc = %lld\n", *pc);
    printf("*pd = %lld\n", *pd);

    return 0;
}
