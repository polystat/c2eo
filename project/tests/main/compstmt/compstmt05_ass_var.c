//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

long long c;
long long d = 10;

int main() {
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
//10
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
//20
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
//30
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
//40
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
  c = d;
//50

  printf("%lld\n", c);
  c;

  return 0;
}
