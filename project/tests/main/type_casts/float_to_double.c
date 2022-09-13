#include <stdio.h>

float f1 = 80.11, f2 = 90.125;
double d1;

int main() {
  d1 = (double) (f1 + f2);
  printf("%f\n", d1);
  return 0;
}