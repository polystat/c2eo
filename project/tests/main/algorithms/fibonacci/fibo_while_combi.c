// Вычиление числа фибоначчи
// Использование глобальных, статических и локальных переменных

#include <stdio.h>

typedef long long i64;

static i64 p1 = 0;
static i64 p2 = 1;

i64 arg;
i64 result;

void fibo() {
  i64 i = 1;
  while (i < arg) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
}

int main() {
  arg = 10;
  fibo();
  printf("%lld\n", arg);
  printf("%lld\n", result);

  return 0;
}
