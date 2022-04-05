//clang -Xclang -ast-dump -fsyntax-only func01.c
//Одна из функций запускается как параметр другой

#include <stdio.h>

long long c = 10;

long long func1(long long x, long long y, long long z) {
    return x - y * z;
}

int main() {
    long long a;
    a = func1(c, 5, 10);

    printf("%lld\n", a);
    a;

    return 0;
}
