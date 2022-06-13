//clang -Xclang -ast-dump -fsyntax-only test07.c

#include "stdio.h"

int main() {
  long long i = 0;
  do
  {
    i++;
    printf("%d\n", i);
    long long j = 0;
    do
    {
      j++;
      printf("%d\n", j);
      long long k = 0;
      do
      {
        k++;
        printf("%d\n", k);
      } while (k < 3);
    } while (j < 3);
  } while (i < 3);
  return 0;
}
