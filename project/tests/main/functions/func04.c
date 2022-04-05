//clang -Xclang -ast-dump -fsyntax-only func01.c
//Одна из функций запускается как параметр другой

#include <stdio.h>

long long c = 10;

long long func1() {
    return c - 111;
}

int main() {
    long long a;
    a = func1();
    printf("%lld\n", a);
    a;
    return 0;
}
