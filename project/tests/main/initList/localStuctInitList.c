//
// Created by yarrya on 18.07.22.
//

#include "stdio.h"

struct st {
  long long x, y, z;
};
typedef struct st st;

int main() {
  st b = {4, 5, 6};
  printf("%lld\n", b.x);
  printf("%lld\n", b.y);
  printf("%lld\n", b.z);
  return 0;
}