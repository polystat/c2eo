//clang -Xclang -ast-dump -fsyntax-only func02.c
//Одна из функций запускается как параметр другой
// Добавлен еще один параметры в первую функцию

#include <stdio.h>

long long c = 10;

long long func1(long long x, long long y) {
  return c + x - y;
}

long long func2(long long x) {
  return c + x - 20;
}

int main() {
  long long a;
  a = func1(func2(c), 30);
  printf("%lld\n", a);

  printf("%lld\n", func1(func2(23), 30));

  return 0;
}
