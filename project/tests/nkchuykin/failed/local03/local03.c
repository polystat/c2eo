//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
int main() {
    int a = 3, b = 4, c = a + b;
    printf("%d\n", c);
    c;
    return 0;
}
