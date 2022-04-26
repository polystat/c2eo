//clang -Xclang -ast-dump -fsyntax-only *.c
#include <stdio.h>

long long a = 10;
long long b = 255;
long long c = 37;

int main() {
  a += c;
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  b = (a += c);
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  b = (a = c);
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  b = a;
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;

  a = b = c;
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  c -= a++;
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  return 0;
}
