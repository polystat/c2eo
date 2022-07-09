#include <stdio.h>

int a = 10;
int b = 0;
int c = 20;

int main() {
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);

  int x;
  x = ~a;
  printf("%d\n", x);
  x = ~b;
  printf("%d\n", x);
  x = ~c;
  printf("%d\n", x);

  return 0;
}
