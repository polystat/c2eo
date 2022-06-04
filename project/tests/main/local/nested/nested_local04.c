//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
int main() {

  for (long long i = 0; i < 5; ++i) {
    long long x = 3;
    x += 1;
    printf("%lld\n", i);
    printf("%lld\n", x);
  }
  return 0;
}
