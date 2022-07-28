
#include "stdio.h"

int main() {
  long long n = 0;
  do {
    n++;
    if (n % 2 == 0) {
      continue;
    }
    printf("%lld\n", n);
    if (n > 5) {
      break;
    }   
  } while (1);
  return 0;
}
