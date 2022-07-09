#include <stdio.h>

int a = 10;
int b = 0;
int c = 20;
int d = 10;
int e = 0;

int main() {
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  printf("%d\n", d);
  printf("%d\n", e);

  int x;
  x = a ^ b;
  printf("%d\n", x);
  x = a ^ c;
  printf("%d\n", x);
  x = a ^ d;
  printf("%d\n", x);
  x = b ^ e;
  printf("%d\n", x);

  return 0;
}
