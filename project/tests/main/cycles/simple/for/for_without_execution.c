#include <stdio.h>

int main() {
  long long i = 1;
  printf("%lld\n", i);
  for (i = 10; 0; i++)
  {
    printf("%lld\n", i);
  }
  printf("%lld\n", i);
}
