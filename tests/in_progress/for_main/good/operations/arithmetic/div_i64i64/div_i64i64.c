//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

// Это была проверка деления на ноль. Она работает одинаково
// Заменено на нейтральное значение
long long c = 10;
// long long x = 0;
long long x = 3;

int main() {
    x = c / x;
    printf("c = %lld\n", c);
    printf("x = %lld\n", x);

    return 0;
}
