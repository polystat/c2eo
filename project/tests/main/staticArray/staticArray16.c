#include "stdio.h"
union st1{
  long long a, b, c;
} obj[3];

int main() {
  obj[1].b = 7;
  printf("%lld\n", obj[1].b);
  return 0;
}