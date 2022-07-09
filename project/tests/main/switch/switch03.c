#include "stdio.h"
long long n = 4;
int main() {
  long long x;
  switch (n) {
  case 1:
    x = 1;
    break;
  case 4:
  case 5:
    x = 5;
    break;
  default:
    x = 13;
    break ;
  }
  printf("%lld",x);
  return 0;
}