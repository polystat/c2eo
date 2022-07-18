//
// Created by yarrya on 18.07.22.
//
#include "stdio.h"

struct st {
  long long x, y;
  struct nst {
    long long a, b;
  } z;
};
typedef struct st st;

st b = {4, 5, {6, 7}};

int main() {
  printf("%lld\n", b.x);
  printf("%lld\n", b.y);
  printf("%lld\n", b.z.a);
  printf("%lld\n", b.z.b);
  return 0;
}