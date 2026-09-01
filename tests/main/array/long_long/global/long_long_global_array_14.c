#include "stdio.h"
union st1{
  long long a[3], b[3], c[3];
} obj;

int main() {
  obj.b[2] = 7;
  printf("%lld\n", obj.b[2]);
  return 0;
}