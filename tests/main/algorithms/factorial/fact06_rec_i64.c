#include <stdio.h>

typedef long long int64;

int64 factorial(int64 n) {
  int64 f = 1;
  if(n < 2) {
    return f;
  }
  else {
    f = n * factorial(n - 1);
    return f;
  }
}

int main() {
  int64 f;
  f = factorial(5);
  printf("%lld\n", f);

  return 0;
}
