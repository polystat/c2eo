//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
long long n = 5;
int main() {
    while (n > 0) {
        printf("%d\n", n);
        n;
        n = n - 1;
    }
    return 0;
}
