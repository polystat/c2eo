//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long b = 5;
long long c = 10;
long long d = 55;
long long e = 66;
long long *pc;

int main() {
  pc = &c;
  pc--;
  e = *pc;

  printf("%lld\n", b);
  printf("%lld\n", c);
  printf("%lld\n", d);
  printf("%lld\n", e);

  return 0;
}
