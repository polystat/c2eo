#include <stdio.h>

_Bool b1 = 1;

int main() {
  printf("%d\n", b1);
  b1 = 0;
  printf("%d\n", b1);
  b1 = 1;
  printf("%d\n", b1);

  return 0;
}
