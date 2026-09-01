
#include "stdio.h"

int main() {
  long long n = 0;
  do {
    n++;
    if (n % 2 == 0) {
      continue;
    }
    printf("%lld\n", n);
  } while (n < 5);
  return 0;
}
