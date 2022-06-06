//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"

int main() {
  long long n = 0;
  while (n < 5) {
    n++;
    if (n % 2 == 0) {
      continue;
    }
    printf("%d\n", n);
  }
    return 0;
}
