#include <stdio.h>

typedef long long int64;

int64 max(int64 a, int64 b) {
  int64 x;
  if(a > b) x = a;
  else x = b;
  return x;
}

// Прототипы функций с их определением
// Алиасы не должны появиться. Функции реализуются
int64 min(int64 a, int64 b);
int64 max(int64 a, int64 b);

int64 x = 10;
int64 y = 7;

int main() {
  int64 result1, result2;
  result1 = min(x, y);
  result2 = max(x, y);

  printf("%lld\n", result1);
  result1;
  printf("%lld\n", result2);
  result2;

  return 0;
}

int64 min(int64 a, int64 b) {
  int64 x;
  if(a < b) x = a;
  else x = b;
  return x;
}
