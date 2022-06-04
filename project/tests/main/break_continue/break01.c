//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
long long n = 5;
int main() {
    while (1) {
        printf("%d\n", n);
        n = n - 1;
        if (n == 0) {
          break;
        }
    }
    return 0;
}
