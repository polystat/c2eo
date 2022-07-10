//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

int a = 10;
int b = 0;
int c = 20;
int d = 10;

int main() {
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  printf("%d\n", d);
  int x;
  x = a & b;
  printf("%d\n", x);
  x = a & c;
  printf("%d\n", x);
  x = a & d;
  printf("%d\n", x);

  return 0;
}
