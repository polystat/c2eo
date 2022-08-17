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

  // Обмен 2-х элементов
  // Основная идея - отработать перемещение через имитацию индексов
  tmp = a[1];
  a[1] = a[0];
  a[0] = tmp;
  printf("a[0] = %lld\n", a[0]);
  printf("a[1] = %lld\n", a[1]);

  return 0;
}

