#include <stdio.h>

float f1 = 80.11, f2 = 90.125;
double d1, d2 = 666.205, d3, d4 = 1200.555;
short s1 = 24, s2 = 65;
int i1 = 105, i2 = 254, i3;
long int li1 = 3333, li2 = 666222, li3;

int main() {
  f1 = (float) (d4 - d2);
  printf("%f\n", f1);
  d1 = (double) (f1 + f2);
  printf("%f\n", d1);
  d3 = (double) (li2 / li1);
  printf("%f\n", d3);
  i3 = (int) (f2 - f1);
  printf("%d\n", s2);
  li3 = (long int) (d2 + d1);
  printf("%d\n", i2);
  li2 = (long int) (d2 * d4);
  printf("%ld\n", li2);
  return 0;
}