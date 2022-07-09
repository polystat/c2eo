#include "stdio.h"

int main() {
  long long i = 0;
  while (i < 3)
  {
    i++;
    printf("%d\n", i);
    long long j = 0;
    while (j < 3)
    {
      j++;
      printf("%d\n", j);
      long long k = 0;
      while (k < 3)
      {
        k++;
        printf("%d\n", k);
      }
    }
  }
  return 0;
}
