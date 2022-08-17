//clang -Xclang -ast-dump -fsyntax-only func01.c

#include <stdio.h>

long long c = 10;

long long func1() {
    return c - 111;
}

int main() {
  long long a;
  a = func1();
  printf("%lld\n", a);

  return 0;
}
