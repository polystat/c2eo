//clang -Xclang -ast-dump -fsyntax-only *.c
#include <stdio.h>

long long a = 10;
long long b = 20;
long long c = 15;

int main() {
  a += c;
  a;
  printf("%lld\n", a);

  b = (a += c);
  a;
  printf("%lld\n", a);
  b;
  printf("%lld\n", b);

  b = (a = c);
  a;
  printf("%lld\n", a);
  b;
  printf("%lld\n", b);

  b = a;
  b;
  printf("%lld\n", b);

  a = b = c;
  b;
  printf("%lld\n", b);
  a;
  printf("%lld\n", a);

  c -= a++;
  a;
  printf("%lld\n", a);
  c;
  printf("%lld\n", c);

  return 0;
}
