#include <stdio.h>

typedef struct matrix{
  struct pair{
    int first;
    int second;
  } first, second;
} matrix;

matrix tmp;

matrix multiply(matrix F, matrix M) {
  int a = F.first.first * M.first.first + F.first.second * M.second.first;
  int b = F.first.first * M.first.second + F.first.second * M.second.second;
  int c = F.second.first * M.first.first + F.second.second * M.second.first;
  int d = F.second.first * M.first.second + F.second.second * M.second.second;
  F.first.first = a;
  F.first.second = b;
  F.second.first = c;
  F.second.second = d;
  return F;
}
matrix power(matrix F, int n) {
  if (n > 1) {
    matrix M = tmp;
    F = power(F, n / 2);
    F = multiply(F, F);
    if (n % 2 != 0)
      F = multiply(F, M);
  }
  return F;
}
int fibonacci_matrix(int n) {
  int ans = 0;
  matrix F = tmp;
  if (n > 0) {
    F = power(F, n - 1);
    ans = F.first.first;
  }
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
  ans;
  return 0;
}