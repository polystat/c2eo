#include <stdio.h>

typedef long long i64;
i64 a[100];
static i64 b[25];
i64 c[] = {1,2,3,4,5};
i64 tmp;

int main() {
  a[0] = 300;
  a[1] = 200;
  printf("a[0] = %lld\n", a[0]);
  printf("a[1] = %lld\n", a[1]);

  // Присваивание скалярам
  i64 x = a[0];
  i64 y = a[1];
  printf("x = %lld\n", x);
  printf("y = %lld\n", y);

  x = a[1];
  y = a[0];
  printf("x = %lld\n", x);
  printf("y = %lld\n", y);

  i64 v = *a;
  i64 w = *(a+1);
  printf("v = %lld\n", v);
  printf("w = %lld\n", w);

  v = *(a+1);
  w = *a;
  printf("v = %lld\n", v);
  printf("w = %lld\n", w);

  return 0;
}

