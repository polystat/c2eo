#include <stdio.h>

long long b = 5;
long long c = 10;
long long d = 55;
long long e = 55;
long long *pc;
long long **ppc;

int main() {
  ++c;
  pc = &c;
  ppc = &pc;
  ++(*ppc);

  e = **ppc;

  printf("%lld\n", b);
  printf("%lld\n", c);
  printf("%lld\n", d);
  printf("%lld\n", e);

  return 0;
}
