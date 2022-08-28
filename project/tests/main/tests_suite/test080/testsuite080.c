//clang -Xclang -ast-dump -fsyntax-only 00080.c
#include <stdio.h>

int c;

void voidfn() {
    c = 10;
    return;
}

int main() {
    voidfn();
    printf("%lld\n", c);
    return 0;
}
