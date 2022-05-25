//clang -Xclang -ast-dump -fsyntax-only main.cpp
#include <stdio.h>
long long a;
long long b;
long long c;
int main() {
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
}
