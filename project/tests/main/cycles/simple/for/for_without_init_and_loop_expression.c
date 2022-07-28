#include <stdio.h>
long long x = 10;

int main() {
  long long i = -1;
  printf("%d\n",i);
  for (;i<x;)
  {
    printf("%d\n",i);
    i++;
  }
  printf("%d\n",i);
}
