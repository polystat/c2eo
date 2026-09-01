#include <stdio.h>

long long a = 6;
long long b = 8;

void print_min(long long x, long long y) {
  if (x < y)
  {
    printf("%lld\n", x);
    return;
  }
  printf("%lld\n", y);
}

int main(){
  print_min(a,b);
  print_min(b,a);
}

