#include <stdio.h>
long long x = 10;

int main() {
  long long i = 1;
  long long j = 2;
  long long k = 0;
  printf("%d\n",i);
  printf("%d\n",j);
  printf("%d\n",k);
  for (i = 0; i<x; i++, j++, k++)
  {
    printf("%d\n",i);
    printf("%d\n",j);
    printf("%d\n",k);
  }
  printf("%d\n",i);
  printf("%d\n",j);
  printf("%d\n",k);
}
