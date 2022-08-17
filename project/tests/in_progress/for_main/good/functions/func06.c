//clang -Xclang -ast-dump -fsyntax-only func02.c
//Одна из функций запускается как параметр другой
// Добавлен еще один параметры в первую функцию

#include <stdio.h>

long long c = 10ll;
//long long d;

long long func1(long long x, long long y) {
    long long t1;
    t1 = c + x - y;
    return t1;
}

long long func2(long long x) {
    return c - x + 20ll;
}

long long func3() {
    return c - 30ll;
}

long long func4() {
  return c - 40ll;
}

int main() {
  long long a;
  a = func1(c, 10ll);
  printf("%lld\n", a);
  a = a + func2(c);
  printf("%lld\n", a);
  a = func1(c, 10ll) + func2(c);
  printf("%lld\n", a);

  printf("%lld\n", c);
  c = func1(c, 10ll);
  printf("%lld\n", c);
  c = c + func2(c);
  printf("%lld\n", c);
  c = func2(c);
  printf("%lld\n", c);
  c = func3();
  printf("%lld\n", c);
  c = func4();
  printf("%lld\n", c);
  c = func3();
  printf("%lld\n", c);
  c = func3();
  printf("%lld\n", c);
  c = c + 5ll;
  printf("%lld\n", c);
  c = func2(c);
  printf("%lld\n", c);
  c = c + 5ll;
  printf("%lld\n", c);
  c = func1(c, 10ll) + func3();
  printf("%lld\n", c);
  c = c + 5ll;
  printf("%lld\n", c);

  return 0;
}
