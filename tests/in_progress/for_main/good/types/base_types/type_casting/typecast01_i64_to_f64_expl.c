#include <stdio.h>

typedef long long int64;

int64  a = 10;
double b = 3.141592;

int main() {
  printf("%lld\n", a);
  printf("%f\n", b);

  b = (double)a;
  printf("%f\n", b);

  return 0;
}

