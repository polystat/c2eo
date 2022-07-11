#include <stdio.h>
long long x = 10;

int main() {
  long long i;
  i = 0;
  printf("%d\n",i);
  for (; i<x; i++)
  {
    printf("%d\n",i);
  }
  printf("%d\n",i);
}