//clang -Xclang -ast-dump -fsyntax-only test07.c

int a;
#include "stdio.h"
int main() {
    int a = 3;
    int b = 4;
    {
        int a;
        int c = a + b;
        printf("%d\n", c);
        c;
    }
    return 0;
}
