#include "stdio.h"
long long n = 5;
int main() {

  long long x;
  long long test = 42;
  switch (n) {
  case 1:
    x = 1;
    break;
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