//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
long long d = 255;
long long *pc;
long long *pd;
long long **ppc;
long long **ppd;

int main() {
  pc = &c;
  ppc = & pc;
  pd = &d;
  ppd = & pd;
  c = *pc;
  c = **ppc;
  pc = &c;
  d = *&c;
  pc = &*&c;
  d = *&*&c;
  pd = pc;
  ppd = ppc;
  pd = &*pc;
  c = *&*pc;
  c = *&**&*ppc;

  printf("%lld\n", c);
  printf("%lld\n", d);

  return 0;
}
