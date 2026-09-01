#include <stdio.h>

int foo(int x) {
  int y = x++ + x++;
  return y;
}

int main() {
  int v = foo(5);
  printf("v = %d\n", v);
  return 0;
}
