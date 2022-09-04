#include <stdio.h>

int a[100] ={1, 2};

int main() {
  a[0] = 300;
  a[1] = 200;
  printf("a[0] = %d\n", a[0]);
  printf("a[1] = %d\n", a[1]);

  int x = a[0];
  int y = a[1];
  printf("x = %d\n", x);
  printf("y = %d\n", y);

  x = a[1];
  y = a[0];
  printf("x = %d\n", x);
  printf("y = %d\n", y);

  return 0;
}

