
#include <stdio.h>

long long a = 10;
long long b = 15;

long long min(long long a, long long b) {
  if (a < b)
    return a;
  return b;
}

int main() {
  long long res;
  res = min(a,b);
  printf("%lld\n", res);
  return 0;
}
