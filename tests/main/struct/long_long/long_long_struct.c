#include <stdio.h>

struct s01 {
  long long a;
};

struct s01 sv;
struct s01 *sp;

int main() {
  sv.a = 10;
  long long x;
  sp = &sv;

  x = sp->a;
  printf("%lld\n", x);

  x = (*sp).a;
  printf("%lld\n", x);

  return 0;
}
