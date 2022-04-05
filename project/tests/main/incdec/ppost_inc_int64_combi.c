//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;

int main() {
  long long x;
  x = c++ + c++;
  printf("%lld\n", c);
  c;
  printf("%lld\n", x);
  x;

  return 0;
}
