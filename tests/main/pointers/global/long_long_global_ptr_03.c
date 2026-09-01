// 1) Assigment the address to pointer
// 2) Assigment the value to variable across pointer

#include <stdio.h>

long long c = 10;
long long d = 255;
long long *pc;
long long *pd;
long long **ppc;
long long **ppd;

int main() {
  pc = &d;
  c = *pc;

  printf("%lld\n", c);
  printf("%lld\n", d);

  return 0;
}
