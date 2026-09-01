#include <stdio.h>

int func01(int a) {
  return a+10;
}

int (*func_ptr)();

int main() {
  int result = 1;

  func_ptr = func01;
  result = func_ptr(result);
  printf("func01 = %d\n", result);

  result = func01(result);
  printf("func01 = %d\n", result);

  return 0;
}
