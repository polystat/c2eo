#include <stdio.h>

long x;
long w;

long test_eg(long x, long y, long z) {
  long w = 1;
  switch (x) {
    case 1:
      w = y * z;
      break;
    case 2:
      w = y / z;
    case 3:
      w += z;
      break;
    case 5:
    case 6:
      w -= z;
      break;
    default:
      w = 2;
  }
  return w;
}

int main() {
  x = 0;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 1;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 2;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 3;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 4;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 5;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 6;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);

  x = 7;
  printf("%ld\n", x);
  w = test_eg(x, 10, 3);
  printf("%ld\n", w);
}
