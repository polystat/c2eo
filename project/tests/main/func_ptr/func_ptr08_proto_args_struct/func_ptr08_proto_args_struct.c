#include <stdio.h>

int func01(int, double, long);

struct rect{long x, y;} r1 = {10, 30};

int func02(int x, struct rect r) {
  return x + r.x + r.y;
}

int (*func_ptr)(int x, double y, long z);
int (*func_ptr2)(int x, struct rect r);

double t = 3.14;
long q = 56;

int main() {
  int result = 12;

  func_ptr = func01;
  func_ptr2 = func02;

  result = func01(result, t, q);
  printf("func01=%d\n", result);

  result = func_ptr(result, t, q);
  printf("func01=%d\n", result);

  result = func02(result, r1);
  printf("func02=%d\n", result);

  result = func01(result, 3.14, 120L);
  printf("func01=%d\n", result);

  result = func_ptr(result + 1, 3.14, 120L);
  printf("func01=%d\n", result);

  result = func_ptr(1, 2.0, 3L);
  printf("func01=%d\n", result);

  result = func_ptr2(result, r1);
  printf("func02=%d\n", result);

  result = func_ptr2(1, r1);
  printf("func02=%d\n", result);

  return 0;
}

int func01(int x, double y, long z) {
  return x + y + z;
}
