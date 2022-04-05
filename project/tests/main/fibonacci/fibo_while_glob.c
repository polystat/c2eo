// Вычисление числа фибоначчи
// Использование глобальных переменных

#include <stdio.h>

typedef long long i64;

i64 p1 = 0;
i64 p2 = 1;
i64 arg;
i64 result;
i64 i;

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
