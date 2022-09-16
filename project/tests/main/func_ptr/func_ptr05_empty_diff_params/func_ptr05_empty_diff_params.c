#include <stdio.h>

int func01();
int (*func_ptr)();

int main() {
  int result = 10;

  func_ptr = func01;

  result = func_ptr();
  printf("func01=%d\n", result);

  result = func_ptr(1);
  printf("func01=%d\n", result);

  result = func_ptr(1,2);
  printf("func01=%d\n", result);

  result = func_ptr(result, result, result);
  printf("func01=%d\n", result);

  result = func_ptr(10, 20, 30);
  printf("func01=%d\n", result);

  result = func01(result);
  printf("func01=%d\n", result);

  return 0;
}

int func01() {
  return 1234;
}

