//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long a = 1;
long long b = 5;
long long c = 10;
long long d = 55;
long long e = 55;
long long *pc;
long long **ppc;

int main() {
  ppc = &pc;
  ++c;
  pc = &c;
  ppc = &pc;
  ++(*ppc);
  ++pc;
  --pc;
  pc++;
  pc--;

  ++ppc;
  --ppc;
  ppc++;
  ppc--;

  e = *pc;
  a = **ppc;
  b = *(++pc);
  printf("%lld\n", a);
  a;
  printf("%lld\n", b);
  b;
  printf("%lld\n", c);
  c;
  printf("%lld\n", d);
  d;
  printf("%lld\n", e);
  e;

  return 0;
}
