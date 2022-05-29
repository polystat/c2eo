// Вычиление числа фибоначчи
// Использование глобальных переменных

#include <stdio.h>

typedef long long i64;

i64 p1 = 0;
i64 p2 = 1;
i64 n;
i64 result;
i64 i;

void fibo() {
  i = 1;
  while (i < n) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
}

int main() {
  n = 10;
  fibo();
  printf("%lld\n", n);
  printf("%lld\n", result);
  return 0;
}