//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"
long long n = 5;
int main() {
  while (1) {
    long long j = 0;
    do {
      for (long long i = 0; 1; ++i) {
        long long res = i * j;
        printf("%lld\n", res);
        if (i >= 5)
          break;
      }
      j++;
      if (j >= 5)
        break;
    } while (1);
    break;
  }
  return 0;
}
