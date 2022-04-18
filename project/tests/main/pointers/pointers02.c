//clang -Xclang -ast-dump -fsyntax-only main.c
// 1) Assigment the address of the variable to pointer
// 2) Assigment the address of the pointer to the pointer on a pointer
// 2) Assigment the value to the variable across the pointer on a pointer
#include <stdio.h>

long long c = 10;
long long d = 255;
long long *pc;
long long *pd;
long long **ppc;
long long **ppd;

int main() {
  pc = &d;
  ppc = &pc;
  c = **ppc;
//   pc = &c;
//   d = *&c;
//   pc = &*&c;
//   d = *&*&c;
//   pd = pc;
//   ppd = ppc;
//   pd = &*pc;
//   c = *&*pc;
//   c = *&**&*ppc;

  printf("%lld\n", c);
  c;
  printf("%lld\n", d);
  d;

  return 0;
}
