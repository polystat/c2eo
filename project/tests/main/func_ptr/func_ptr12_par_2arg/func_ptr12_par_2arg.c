#include <stdio.h>

int func01(int x, long y, int z) {
  return x + y + z;
}

int (*func_ptr)(int, long, int);

int main() {
  int result;

  func_ptr = func01;
  result = func_ptr(3, 4, 5);
  printf("func01 = %d\n", result);

  result = func01(result + 5, result + 7, result - 3);
  printf("func01 = %d\n", result);

  return 0;
}
