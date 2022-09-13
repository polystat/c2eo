#include <stdio.h>

int a = 5;
int b = 6;

int main() {
  a = 300;
  printf("a = %d\n", a);
  a = 301;
  printf("a = %d\n", a);
  a = 302;
  printf("a = %d\n", a);
  b = 1002;
  printf("b = %d\n", b);

  int x = a;
  int y = a;
  printf("x = %d\n", x);
  printf("y = %d\n", y);

  x = a;
  y = a;
  printf("x = %d\n", x);
  printf("y = %d\n", y);

  int v = b;
  int w = b;
  printf("v = %d\n", v);
  printf("w = %d\n", w);

  v = b;
  w = b;
  printf("v = %d\n", v);
  printf("w = %d\n", w);


  return 0;
}
