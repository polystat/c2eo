#include <stdio.h>

struct s01 {
  long long a, b;
} a = {1, 2};

struct s01 b = {3, 4};

int main() {
  struct s01 c = {5, 6};
  printf("%lld\n", a.a);
  printf("%lld\n", a.b);
  printf("%lld\n", b.a);
  printf("%lld\n", b.b);
  printf("%lld\n", c.a);
  printf("%lld\n", c.b);


  return 0;
}