//clang -Xclang -ast-dump -fsyntax-only test07.c

long long a;
#include "stdio.h"
int main() {
    long long a = 3;
    long long b = 4;
    {
        long long a;
        long long c = a + b;
        printf("%d\n", c);
    }
    return 0;
}
