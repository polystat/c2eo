#include "stdio.h"

long long a[3] = {1,2,3};
int main() {
  long long b[3] = {4,5,6};
  printf("%lld\n", a[0]);
  printf("%lld\n", a[1]);
  printf("%lld\n", a[2]);
  printf("%lld\n", b[0]);
  printf("%lld\n", b[1]);
  printf("%lld\n", b[2]);
  return 0;
}