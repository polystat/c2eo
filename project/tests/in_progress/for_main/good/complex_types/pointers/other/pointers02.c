//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c;
long long d;
long long *pc;

int main() {
    c = 255;
    c = -c;
    d = -+-+-c;
    pc = &c;
    d = *pc;
    c = *&*pc;
    c = *(&(*pc));
    d = *(&c);
    d = *&c;
    printf("c = %lld\n", c);
    printf("d = %lld\n", d);
    printf("*pc = %lld\n", *pc);

    return 0;
}
