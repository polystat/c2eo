#include <stdio.h>
//#include <stdint.h>
typedef int int32_t;
typedef long long int64_t;

int test() {
  int32_t x;
  int64_t l;

  x = 0;
  l = 0;
  printf("%d\n", x);
  printf("%lld\n", l);

  x = ~x;
  printf("%d\n", x);
  if (x != 0xffffffff)
    return 1;

  l = ~l;
  printf("%lld\n", l);
  if (x != 0xffffffffffffffff)
    return 2;

  return 0;
}

#include <stdio.h>
int main() {
  int x = test();
  printf("%d\n", x);
  return x;
}
