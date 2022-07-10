#include <stdio.h>



int fibo(int p1, int p2, int n) {
  int tmp;
  if (0 < n) {
    tmp = p1;
    p1 = p1 + p2;
    p2 = tmp;
    n--;
    return fibo(p1, p2, n);
  } else {
    return p1;
  }
}

int main() {
  int r = fibo(0, 1, 10);
  printf("%d\n", r);
  return 0;
}
