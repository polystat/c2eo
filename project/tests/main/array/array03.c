#include "stdio.h"

static long long a[5];
static long long i = 2;
int main() {
  a[i] = 33;
  printf("%d\n", a[i]);

  return 0;
}
