//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

double d = 255;

int main() {
  printf("d = %f\n", d);
  d--;
  printf("d-- = %f\n", d);

  return 0;
}
