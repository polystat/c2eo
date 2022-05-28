//clang -Xclang -ast-dump -fsyntax-only func02.c
//Одна из функций запускается как параметр другой
// Добавлен еще один параметры в первую функцию

#include <stdio.h>

long long c = 10ll;

long long func1(long long x, long long y) {
    long long t1 = c + x - y;
    return t1;
}

long long func2(long long x) {
    return c + x - 10ll;
}

int main() {
    long long a;
    a = func1(c, 10ll);
    a = a + func2(c);
    a = func1(c, 10ll) + func2(c);
    printf("%lld\n", a);
    return 0;
}
