#include <stdio.h>

void test() {
  // empty statement
  ;
}

int main() {
  int x = 0;
  test();
  printf("%d\n", x);

  return 0;
}
