//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
int main() {
  long long a = 3;
  {
    long long a = 7;
    a += 2;
    printf("%lld\n", a);
  }
  a += 2;
  printf("%lld\n", a);
  return 0;
}
