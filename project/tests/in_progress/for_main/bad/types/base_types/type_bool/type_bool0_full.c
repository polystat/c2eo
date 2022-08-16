#include <stdio.h>

_Bool b1 = 1;
int   i1 = 23;

int main() {
  printf("%d\n", b1);
  printf("%d\n", i1);
  b1 = i1;
  i1 = b1;
  printf("%d\n", b1);
  printf("%d\n", i1);
  b1 = 1024;
  printf("%d\n", b1);
  b1 = 2049;
  printf("%d\n", b1);
  i1 = 4096;
  b1 = i1;
  printf("%d\n", b1);
  printf("%d\n", i1);
  i1 = -1;
  b1 = i1;
  printf("%d\n", b1);
  printf("%d\n", i1);
  i1 = 0;
  b1 = i1;
  printf("%d\n", b1);
  printf("%d\n", i1);

  return 0;
}
