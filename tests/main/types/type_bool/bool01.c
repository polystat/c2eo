//clang -Xclang -ast-dump -fsyntax-only main.c
#include "stdio.h"
_Bool b1 = 1;
_Bool b2 = 0;
int main() {
    printf("%d\n", b1);
    printf("%d\n", b2);
    return 0;
}
