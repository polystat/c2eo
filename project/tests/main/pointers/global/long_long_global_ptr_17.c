//#include <stdio.h>

long long a = 10;
long long c;
long long *pc;
long long **ppc;

int main() {
  pc = &a;
  ppc = &pc;
  c = *&**&*ppc;
  printf("%lld\n", c);

  return 0;
}
