//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>

long long x = 5;
long long y = 10;
int main() {
    x += y;
    x;
    printf("%d\n", x);
    x *= y;
    x;
    printf("%d\n", x);
}
