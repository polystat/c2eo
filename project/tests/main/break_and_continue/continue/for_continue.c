
#include "stdio.h"

int main() {
  for (long long n = 0; n < 5; n++) {
    if (n % 2 == 0) {
      continue;
    }
    printf("%lld\n", n);
  }
    return 0;
}
