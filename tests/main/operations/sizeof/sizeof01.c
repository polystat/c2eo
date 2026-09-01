#include <stdio.h>

int i1 = 0;

int main() {
  i1 = sizeof(int);
  printf("%d\n", i1);

  i1 = sizeof(long int);
  printf("%d\n", i1);

  return 0;
}
