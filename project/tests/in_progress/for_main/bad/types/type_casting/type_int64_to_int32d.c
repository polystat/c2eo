#include <stdio.h>

int i1 = 0x98765432;
long int i2 = 0x9876543210123456l;

int main() {
  printf("%d\n", i1);
  printf("%ld\n", i2);
//   printf("%d\n", (int)i2);
  i1 = -10000 + (int)i2;
  printf("%d\n", i1);
  return 0;
}
