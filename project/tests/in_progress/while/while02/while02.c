//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
int n = 5;
int main() {
    printf("%d\n", n);
    n;
    while (n > 0)
        n = n - 1;
    printf("%d\n", n);
    n;
    return 0;
}
