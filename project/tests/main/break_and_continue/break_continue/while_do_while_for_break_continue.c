#include "stdio.h"
long long n = 5;
int main() {
  while (1) {
    long long j = 0;
    do {
      for (long long i = 0; 1; ++i) {
        long long res = i * j;
        if (i % 2 == 0) {
          continue;
        }
        printf("%lld\n", res);
        if (i >= 5)
          break;
      }
      j++;
      if (j >= 5) {
        break;
      } else {
        continue;
      }
    } while (1);
    break;
    continue;
  }
  return 0;
}
