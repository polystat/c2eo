#include <stdio.h>

long long c = 10;
long long d = 55;
long long e = 66;
long long *pc;
long long **ppc;

int main() {
  pc = &c;
  ++pc;
  e = *pc;

  printf("%lld\n", c);
  printf("%lld\n", d);
  printf("%lld\n", e);

  return 0;
}
