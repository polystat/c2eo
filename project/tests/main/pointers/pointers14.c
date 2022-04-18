//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 255;
long long e = 333;
long long *pc;
long long *pd;
long long **ppc;
long long **ppd;

int main() {
  pc = &d;
  c = c + 1;
//   c = *(pc + 1);
  pd = pc + 1;

  printf("%lld\n", c);
  c;
  printf("%lld\n", d);
  d;
  printf("%lld\n", e);
  e;

  return 0;
}
