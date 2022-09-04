#include <stdio.h>

int a = 10, b = 4, number = 70, i;

int main() {
  double e = (double)a / (double)b;
  printf("%f\n", e); // 2.50000
  char symbol = (char) number;
  printf("%d\n", symbol); // 70
  char c = 45;
  i = (int)c;
  printf("%d\n", i); // 45
  return 0;
}