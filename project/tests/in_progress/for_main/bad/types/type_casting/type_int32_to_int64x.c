#include <stdio.h>

int i1 = 0x98765432;
long int i2 = 0x9876543210123456l;

int main() {
  printf("%x\n", i1);
  printf("%lx\n", i2);
//  printf("%lx\n", (long int)i1);
  i2 = (long int)i1;
  printf("%lx\n", i2);
  return 0;
}
