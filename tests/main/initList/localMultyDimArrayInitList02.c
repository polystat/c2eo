//
// Created by yarrya on 18.07.22.
//
#include "stdio.h"

int main() {
  long long b[3][2] = {{1,2},{3,4},{5,6}};
  printf("%lld\n", b[0][0]);
  printf("%lld\n", b[0][1]);
  printf("%lld\n", b[1][0]);
  printf("%lld\n", b[1][1]);
  printf("%lld\n", b[2][0]);
  printf("%lld\n", b[2][1]);
  return 0;
}