//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
long long a;
long long b;
long long c;
int main() {
    a;
    printf("%d\n", a);
    b;
    printf("%d\n", b);
    c;
    printf("%d\n", c);
}
