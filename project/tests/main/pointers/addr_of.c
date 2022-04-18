//clang -Xclang -ast-dump -fsyntax-only main.c
#include <stdio.h>

// Проверка инициализации указателей
long long c = 255;
long long d = 10;
long long *pc; // = &c;

int main() {
  pc = &c;
  d = *pc;
  printf("%lld\n", d);
  d;

  return 0;
}
