//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"

int main() {
  long long i = 0;
  while (i < 3) {
    i++;
    if (i % 2 == 0) {
      continue;
    }
    printf("%d\n", i);
    long long j = 0;
    while (j < 3) {
      j++;
      if (j % 2 == 0) {
        continue;
      }
      printf("%d\n", j);
      long long k = 0;
      while (k < 3) {
        k++;
        if (k % 2 == 0) {
          continue;
        }
        printf("%d\n", k);
      }
    }
  }
  return 0;
}