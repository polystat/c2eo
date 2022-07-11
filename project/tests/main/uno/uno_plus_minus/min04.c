#include <stdio.h>

long long c = 10;

int main() {
  printf("%lld\n", c);
  long long x;
  x = -c + -c;
  printf("%lld\n", x);

  return 0;
}
