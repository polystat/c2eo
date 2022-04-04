//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 10;
double d = 255;
char e = 'z';

int main() {
  c--;
  printf("%lld\n", c);
  c;
//   d--;
//   printf("%g\n", d);
//   d;
//   e--;
//   printf("%c\n", e);
//   e;
  // long long x = c++ + c++;
  // printf("%lld\n", x);

  return 0;
}
