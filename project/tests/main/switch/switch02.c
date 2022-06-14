//clang -Xclang -ast-dump -fsyntax-only test07.c
#include "stdio.h"
long long n = 5;
int main() {
  for (long long i = 0; i < 10; ++i) {
    long long x=0;
    switch (i) {
    case 1:
      x += 1;
      break;
    case 2:
    case 3:
      x += 2;
      break;
    case 4:
      x+= 4;
    case 5:
      x+= 8;
      break;
    default:
      x +=100;
      break ;
    }
    printf("%lld\n",x);
  }

  return 0;
}