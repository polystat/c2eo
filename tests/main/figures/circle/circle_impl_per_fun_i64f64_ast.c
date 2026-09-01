// Вычисление периметра окружности
#include <stdio.h>

typedef long long int64;

double const pi = 3.141592;

// круг
struct circle {
  int64 r;  // радиус
};

struct circle c;

double perimeter(struct circle *c) {
  return 2.0 * pi * (*c).r;
}

int main() {
  c.r = 10;

  double p;
  p = perimeter(&c);

  printf("%f\n", p);

  return 0;
}

