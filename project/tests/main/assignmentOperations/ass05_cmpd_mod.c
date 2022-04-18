//clang -Xclang -ast-dump -fsyntax-only *.c
#include <stdio.h>

long long a = 100;
long long b = 20;
long long c = 15;

int main() {
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;

  a %= c;
  printf("%lld\n", a);
  a;

  return 0;
}
