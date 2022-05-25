//clang -Xclang -ast-dump -fsyntax-only *.c
#include <stdio.h>

long long a = 10;
long long b = 255;
long long c = 37;

int main() {
  a += c;
  printf("%lld\n", a);
  printf("%lld\n", b);
  printf("%lld\n", c);

  b = (a += c);
  printf("%lld\n", a);
  printf("%lld\n", b);
  printf("%lld\n", c);

  b = (a = c);
  printf("%lld\n", a);
  printf("%lld\n", b);
  printf("%lld\n", c);

  b = a;
  printf("%lld\n", a);
  printf("%lld\n", b);

  a = b = c;
  printf("%lld\n", a);
  printf("%lld\n", b);
  printf("%lld\n", c);

  c -= a++;
  printf("%lld\n", a);
  printf("%lld\n", b);
  printf("%lld\n", c);

  return 0;
}
