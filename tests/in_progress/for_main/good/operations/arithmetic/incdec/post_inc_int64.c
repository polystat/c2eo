//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;

int main() {
  c++;
  printf("%lld\n", c);

  return 0;
}
