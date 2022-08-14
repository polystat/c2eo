//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c;
long long d = 10;
long long *pc = &d;

int main() {
    c = 255;
    c = -c;
    pc = &c;
    d = c;
    c++;
    --c;
    printf("c = %lld\n", c);
    printf("*pc = %lld\n", *pc);
    pc;
    long long x;
    long long *px;
    px = &x;
    x = *pc;
    printf("x = %lld\n", x);
    printf("*px = %lld\n", *px);

    return 0;
}
