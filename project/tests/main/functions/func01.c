//clang -Xclang -ast-dump -fsyntax-only func01.c
//Одна из функций запускается как параметр другой

#include <stdio.h>

long long c = 10;

long long func1(long long x) {
    return c + x - 111;
}

long long func2(long long x) {
    return c + x - 10;
}

int main() {
    long long a;
    a = func1(func2(c));
    printf("%lld\n", a);
    a;
    return 0;
}
