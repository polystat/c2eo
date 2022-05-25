//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c = 0;
long long d = 1;

int main() {
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
//10
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
//20
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
//30
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
//40
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  c = c + d;
  //50

  printf("%lld\n", c);

  return 0;
}
