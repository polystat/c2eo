//clang -Xclang -ast-dump -fsyntax-only test07.c
#include "stdio.h"
long long n = 5;
int main() {
  long long x;
  switch (n) {
  case 1:
    x = 1;
    break;
  case 4:
  case 5:
    x = 5;
  default:
    x += 13;
    break ;
  }
  printf("%lld",x);
  return 0;
}