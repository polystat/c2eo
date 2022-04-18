//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

void zero_div(long long c, long long x) {
  printf("%lld\n", x);
  x;

  x = c / x;

  printf("%lld\n", c);
  c;
  printf("%lld\n", x);
  x;
}

int main() {
  zero_div(15, 3);

  long long c = 120;
  long long x = 10;
  zero_div(c, x);

  return 0;
}
