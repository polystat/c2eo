// clang -Xclang -ast-dump -fsyntax-only enum01.c
#include "stdio.h"

enum en1 {
    RED = 1,
    YELLOW = 2,
    GREEN = 3
};

long long color = 3;
long long x = 10;

int main() {
  printf("%lld\n", x);
  if (color == GREEN) {
    x = x * x;
  }
  printf("%lld\n", x);
  return 0;
}
