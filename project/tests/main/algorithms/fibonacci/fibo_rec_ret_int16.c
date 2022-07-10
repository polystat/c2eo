#include <stdio.h>

typedef short int i16;

i16 fibo(i16 p1, i16 p2, i16 n) {
  i16 tmp;
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
  i16 r = fibo(0, 1, 10);
  printf("%hd\n", r);
  return 0;
}
