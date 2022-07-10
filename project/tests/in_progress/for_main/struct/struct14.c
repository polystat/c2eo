#include <stdio.h>

typedef struct matrix{
  struct pair{
    int first;
    int second;
  } first, second;
} matrix;

matrix tmp, tmp2;

int fibonacci_matrix(int n) {
  matrix F;
  F.first.first = tmp.first.first;
  F.first.second = tmp.first.second;
  F.second.first = tmp.second.first;
  F.second.second = tmp.second.second;
  while (n > 1) {
    tmp2.first.first = F.first.first * tmp.first.first + F.first.second * tmp.second.first;
    tmp2.first.second = F.first.first * tmp.first.second + F.first.second * tmp.second.second;
    tmp2.second.first = F.second.first * tmp.first.first + F.second.second * tmp.second.first;
    tmp2.second.second = F.second.first * tmp.first.second + F.second.second * tmp.second.second;
    F.first.first = tmp2.first.first;
    F.first.second = tmp2.first.second;
    F.second.first = tmp2.second.first;
    F.second.second = tmp2.second.second;
    n = n - 1;
  }
  int ans = F.first.first;
  return ans;
}

int main() {
  tmp.first.first=1;
  tmp.second.first=1;
  tmp.first.second=1;
  tmp.second.second=0;
  int n = 5;
  int ans = fibonacci_matrix(n);
  printf("%d\n", ans);
  return 0;
}
