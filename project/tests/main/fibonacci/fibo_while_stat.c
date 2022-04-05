// Вычиление числа фибоначчи
// Использование глобальных и статических переменных

#include <stdio.h>

typedef long long i64;

static i64 p1 = 0;
static i64 p2 = 1;
static i64 i;

i64 arg;
i64 result;

void fibo() {
  i = 1;
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
  arg;
  printf("%lld\n", result);
  result;
  return 0;
}
