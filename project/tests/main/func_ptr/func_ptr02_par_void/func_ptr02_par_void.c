#include <stdio.h>

int func01(void) {
  return 1234;
}

int (*func_ptr)(void);

int main() {
  int result;

  func_ptr = func01;
  result = func_ptr();
  printf("func01 = %d\n", result);

  result = func01();
  printf("func01 = %d\n", result);

  return 0;
}
