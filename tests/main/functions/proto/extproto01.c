#include <stdio.h>

typedef long long int64;

// Прототипы функций без их использования.
// Должны потеряться
int64 min(int64 a, int64 b);
int64 max(int64 a, int64 b);

int64 x = 10;
int64 y = 7;

int main() {
  int64 result;
  result = 0;
  printf("%lld\n", result);
//   result = min(5, 2);

  return 0;
}

