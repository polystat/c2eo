//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
int n = 5;
int main() {
    do {
        printf("%d\n", n);
        n;
        n = n - 1;
    } while (n > 0);
    return 0;
}
