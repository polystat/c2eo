#include "stdio.h"

int main() {
  int x = -5;

  int res = x > 0 ? x : -x;

  printf("%d\n", res);
  //printf("%s\n", res % 2 ? "even" : "odd");

  return 0;
}