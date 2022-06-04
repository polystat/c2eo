//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"

int main() {
    long long n = 5;
    for ( ;1; n--) {
        printf("%d\n", n);
        if (n == 0) {
          break;
        }
    }
    return 0;
}
