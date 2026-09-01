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

int small(int n) {
  if (n == 2) {
    return 1;
  } else {
    return n;
  }
}

int rec(int n, int minus1, int minus2) {
  if (n == 3) {
    return minus1 + minus2;
  } else {
    return rec(n - 1, minus1 + minus2, minus1);
  }
}

int fibonacci(int n) {
  if (n < 3) {
    return small(n);
  } else {
    return rec(n, 1, 1);
  }
}

int main() {
  int fib;
  //fib = fibonacci(46);
  fib = fibonacci(10);

  printf("fibonacci = %d\n", fib);
  fib;
}
