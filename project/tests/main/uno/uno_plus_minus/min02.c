//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;

int main() {
  printf("%lld\n", c);
  c;
  long long x;
  x = -(-c);
  printf("%lld\n", x);
  x;

  return 0;
}
