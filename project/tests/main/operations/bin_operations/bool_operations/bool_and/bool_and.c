#include <stdio.h>

int a = 10;
int b = 0;
int c = 20;

int main() {
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  int x;
  x = a && b;
  printf("%d\n", x);
  x = a && c;
  printf("%d\n", x);

  return 0;
}
