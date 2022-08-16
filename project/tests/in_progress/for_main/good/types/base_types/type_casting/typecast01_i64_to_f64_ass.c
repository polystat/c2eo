#include <stdio.h>

typedef long long int64;
typedef double    float64;

int64   a = 10;
float64 b = 3.141592;

int main() {
  printf("%lld\n", a);
  printf("%f\n", b);

  b = a;
  printf("%f\n", b);

  return 0;
}

