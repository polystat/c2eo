#include "stdio.h"

long long test_eg(long long x, long long y, long long z) {
    long long r;
    if (x > y)
    {
        if (x > z)
            r = x;
        else
            r = z;
    } else {
        if (y > z)
            r = y;
        else
            r = z;
    }
    return r;
}

long long w;

int main() {
  w = test_eg(1, 2, 3);
  printf("%lld\n", w);

  w = test_eg(1, 3, 2);
  printf("%lld\n", w);

  w = test_eg(2, 1, 3);
  printf("%lld\n", w);

  w = test_eg(2, 3, 1);
  printf("%lld\n", w);

  w = test_eg(3, 1, 2);
  printf("%lld\n", w);

  w = test_eg(3, 2, 1);
  printf("%lld\n", w);
  return 0;
}
