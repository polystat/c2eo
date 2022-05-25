//clang -Xclang -ast-dump -fsyntax-only *.c
#include <stdio.h>

long long a = 10;
long long b = 255;
long long c = 37;

int main() {
  a = b = c;
  printf("%lld\n", a);
  printf("%lld\n", b);
  printf("%lld\n", c);

  return 0;
}
