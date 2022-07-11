#include "stdio.h"

enum en2 {
    DOG = 3,
    MOUSE,
    CAT
};

long long animal = 2;
long long x = 10;

int main() {
  printf("%lld\n", x);
  if (animal == MOUSE) {
    x = x * x;
  }
  printf("%lld\n", x);
}
