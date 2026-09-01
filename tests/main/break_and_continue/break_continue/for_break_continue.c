
#include "stdio.h"

int main() {
  for (long long n = 0;1;n++) {
    if (n % 2 == 0) {
      continue;
    }
    printf("%lld\n", n);
    if (n > 5) {
      break;
    }   
  }
  return 0;
}
