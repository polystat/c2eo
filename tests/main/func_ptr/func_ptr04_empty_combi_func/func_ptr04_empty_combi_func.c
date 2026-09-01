#include <stdio.h>

int func01() {
  return 1234;
}

int func02(int a) {
  return 1234 + a;
}

int (*func_ptr)();

int main() {
  int result = 10;

  func_ptr = func01;
  result = func_ptr();
  printf("func01=%d\n", result);

  func_ptr = func02;
  result = func_ptr(result);
  printf("func01=%d\n", result);

  result = func02(result);
  printf("func01=%d\n", result);

  return 0;
}
