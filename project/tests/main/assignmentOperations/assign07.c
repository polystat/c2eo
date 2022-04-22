//clang -Xclang -ast-dump -fsyntax-only *.c
#include <stdio.h>

long long a = 10;
long long b = 255;
long long c = 37;

int main() {
  b = (a += c);
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  return 0;
}
