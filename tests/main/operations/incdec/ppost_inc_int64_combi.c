#include <stdio.h>

long long c = 10;

int main() {
  long long x;
  x = c++ + c++;
  printf("%lld\n", c);
  printf("%lld\n", x);

  return 0;
}
