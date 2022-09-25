#include <stdio.h>

int func01(void);

int (*func_ptr)();
//int (*func_ptr)(int x, double y, long z);

int main() {
  int result = func01();
  printf("%d\n", result);
  func_ptr = func01;
  result = func_ptr(result, 3, 2);
  printf("%d\n", result);

  return 0;
}

int func01(void) {
  return 1010;
}
