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
  printf("%lld\n", b);
  printf("%lld\n", c);
  printf("%lld\n", d);
  printf("%lld\n", e);

  return 0;
}
