#include <stdio.h>
long long x = 4;

int main() {
  long long i = 1;
  printf("%d\n",i);
  for (long long j = 10, k = 45; i<x; i++)
  {
    printf("%d\n",i);
    printf("%d\n",j);
    printf("%d\n",k);
  }
  printf("%d\n",i);
}
