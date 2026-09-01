#include <stdio.h>
long long x = 10;

int main() {
  long long i = -1;
  printf("%d\n",i);
  for (;;i++)
  {
    if(i<x) {
        printf("%d\n",i);
    } else {
        break;
    }
  }
  printf("%d\n",i);
}