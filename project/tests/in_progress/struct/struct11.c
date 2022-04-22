//#include <stdio.h>

typedef struct matrix{
  struct pair{
    int first;
    int second;
  } first, second;
} matrix;

matrix tmp, tmp2;

//matrix multiply(matrix F, matrix M) {
//  int a = F.first.first * M.first.first + F.first.second * M.second.first;
//  int b = F.first.first * M.first.second + F.first.second * M.second.second;
//  int c = F.second.first * M.first.first + F.second.second * M.second.first;
//  int d = F.second.first * M.first.second + F.second.second * M.second.second;
//  F.first.first = a;
//  F.first.second = b;
//  F.second.first = c;
//  F.second.second = d;
//  return F;
//}
//matrix power(matrix F, int n) {
//  if (n > 1) {
//    matrix M;
//    M = tmp;
//    F = power(F, n / 2);
//    F = multiply(F, F);
//    if (n % 2 != 0)
//      F = multiply(F, M);
//  }
//  return F;
//}
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
//  printf("%d\n", ans);
  ans;
  return 0;
}