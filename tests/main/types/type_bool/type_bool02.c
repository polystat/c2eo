#include <stdio.h>

_Bool b1 = 0;
int   i0 = 0;
int   i1 = 1;

int main() {
  printf("%d\n", b1);
  b1 = i1;
  printf("%d\n", b1);
  b1 = i0;
  printf("%d\n", b1);

  return 0;
}
