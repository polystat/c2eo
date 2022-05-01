//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 55;
long long e = 66;
long long *pc;

int main() {
  pc = &c;
  pc++;
  e = *pc;

  printf("%lld\n", c);
  c;
  printf("%lld\n", d);
  d;
  printf("%lld\n", e);
  e;

  return 0;
}
