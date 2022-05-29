// Вычиление числа фибоначчи
// Использование глобальных, статических и локальных переменных
// а также передачи параметров и возврат результата
#include <stdio.h>

typedef long long i64;

i64 fibo(i64 n) {
  i64 p1 = 0;
  i64 p2 = 1;
  i64 result = 1;
  i64 i = 1;
  while (i < n) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
  return result;
}

int main() {
  i64 n = 10;
  i64 result = fibo(n);
  printf("%lld\n", n);
  printf("%lld\n", result);
  return 0;
}