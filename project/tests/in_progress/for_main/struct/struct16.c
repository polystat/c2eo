#include <stdio.h>

typedef struct matrix {
  struct pair {
    int first;
    int second;
  } first, second;
} matrix;

matrix tmp;

int main() {
  tmp.first.first = 100;
  tmp.second.first = 100;
  tmp.first.second = 100;
  tmp.second.second = 100;
  int n = 5;
  int ans = (((n + tmp.first.first * tmp.second.second - tmp.first.second * tmp.second.first * tmp.second.second)
      >> (n - 1)) + tmp.second.second);
  printf("%d\n", ans);
  return 0;
}
