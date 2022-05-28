#include <stdio.h>

typedef long long i64;

i64 fibo(i64 p1, i64 p2, i64 n) {
  i64 tmp;
  if (0 < n) {
    tmp = p1;
    p1 = p1 + p2;
    p2 = tmp;
    n--;
    return fibo(p1, p2, n);
  }
  return p1;
}

int main() {
  i64 r;
  r = fibo(0, 1, 10);
  printf("%lld\n", r);
  return 0;
}
