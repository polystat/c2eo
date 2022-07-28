#include <stdio.h>

// Проверка инициализации указателей
long long c = 255;
long long *pc; // = &d;
long long *pd; // = &d;

int main() {
  pc = &c;
  pd = pc;
  c = *pd;
  printf("%lld\n", c);

  return 0;
}
