#include <stdio.h>

typedef long long int64;

// треугольник
struct triangle {
  int64 a, b, c; // ширина, высота
};

struct triangle t;

double perimeter(struct triangle t) {
  return t.a + t.b + t.c;
}

int main() {
  t.a = 30;
  t.b = 40;
  t.c = 50;

  double p;
  p = perimeter(t);

  printf("%f\n", p);

  return 0;
}

