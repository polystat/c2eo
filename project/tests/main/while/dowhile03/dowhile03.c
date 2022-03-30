//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
long long n = 5;
int main() {
    printf("%d\n", n);
    n;
    do
        n = n - 1;
    while (n > 6);
    printf("%d\n", n);
    n;
    return 0;
}
