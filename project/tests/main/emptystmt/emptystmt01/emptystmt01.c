//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

int test() {
  // empty statement
  ;;;
  return 0;
}

int main() {
  int x;
  x = test();
  printf("%d\n", x);

  return 0;
}
