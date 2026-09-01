#include <stdio.h>

typedef long long int64;

int64 factorial(int64 n) {
  if(n < 2) {
    return n;
  }
  else {
    return n * factorial(n -1);
  }
}

int main() {
  int64 f;
  f = factorial(5);
  printf("%lld\n", f);
  f;

  return 0;
}
