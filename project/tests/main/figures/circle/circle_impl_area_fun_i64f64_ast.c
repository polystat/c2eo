// Вычисление площади круга
#include <stdio.h>

typedef long long int64;

double const pi = 3.141592;

// круг
struct circle {
  int64 r;  // радиус
};

struct circle c;

double area(struct circle *c) {
  return pi * (*c).r * (*c).r;
}

int main() {
  c.r = 10;

  double p;
  p = area(&c);

  printf("%f\n", p);
  p;

  return 0;
}

