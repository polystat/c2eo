//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
int main() {
  for (long long i = 0; i < 5; ++i) {
    printf("%lld\n", i);
  }
  return 0;
}
