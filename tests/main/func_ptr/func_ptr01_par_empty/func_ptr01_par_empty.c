#include <stdio.h>

int func01() {
  return 1234;
}

int (*func_ptr)();

int main() {
  int result = 1;

  func_ptr = func01;
  result = func_ptr(result);
  printf("func01 = %d\n", result);

  result = func01();
  printf("func01 = %d\n", result);

  return 0;
}
