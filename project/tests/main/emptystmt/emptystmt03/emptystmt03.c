//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

void test() {
  // empty statement
  ;
  ;
  ;;;
}

int main() {
  int x = 0;
  test();
  printf("%d\n", x);

  return 0;
}
