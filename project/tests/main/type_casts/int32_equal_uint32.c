#include <stdio.h>

int any0, any1 = 2147483640, any2 = 1000;
unsigned int positive0, positive1 = 4294967295, positive2 = 2000;

int main() {
  positive0 = (unsigned int) any1 - any2;
  printf("%d\n", positive0);
  any0 = (int) positive1 - positive2;
  printf("%d\n", any0);
  return 0;
}