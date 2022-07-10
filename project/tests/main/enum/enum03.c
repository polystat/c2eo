#include "stdio.h"

enum en3 {
    ZERO,
    ONE,
    TWO
};

long long x = 10;

int main() {
  printf("%lld\n", x);
  for (int i = 0; i < 3; ++i) {
    if (i == ONE) {
      x = x * x;
    }
  }
  printf("%lld\n", x);
}
