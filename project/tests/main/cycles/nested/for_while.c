#include "stdio.h"

int main() {
  for (long long i=0; i<3; i++)
  {
    printf("%d\n",i);
    long long j = 3;
    printf("%d\n", j);
    while (j > 0) {
        j--;
        printf("%d\n", j);
    }
    printf("%d\n", j);
    printf("%d\n", i);
  }
}
