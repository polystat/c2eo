/*
+package sandbox

[n] > fibonacci
  if. > @
    n.less 3
    small n
    rec n 1 1

  [n] > small
    if. > @
      n.eq 2
      1
      n

  [n minus1 minus2] > rec
    if. > @
      n.eq 3
      minus1.add minus2
      rec (n.sub 1) (minus1.add minus2) minus1
*/

#include <stdio.h>

typedef long long int64;

int64 small(int64 n) {
  if (n == 2) {
    return 1;
  } else {
    return n;
  }
}

int64 rec(int64 n, int64 minus1, int64 minus2) {
  if (n == 3) {
    return minus1 + minus2;
  } else {
    return rec(n - 1, minus1 + minus2, minus1);
  }
}

int64 fibonacci(int64 n) {
  if (n < 3) {
    return small(n);
  } else {
    return rec(n, 1, 1);
  }
}

int64 main() {
  int64 fib;
  fib = fibonacci(10);

  printf("%lld\n", fib);
  fib;

  return 0;
}
