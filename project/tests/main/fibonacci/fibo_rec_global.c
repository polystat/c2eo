// Рекурсивное вычисление числа фибоначчи
// Использование глобальных переменных

#include <stdio.h>

typedef long long i64;

i64 p1 = 0;
i64 p2 = 1;
i64 arg;
i64 result = 0;

void fibo() {
  if (1 < arg) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
//     arg--;
    arg = arg - 1;
    fibo();
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
