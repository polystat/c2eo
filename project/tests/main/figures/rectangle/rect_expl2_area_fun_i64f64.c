#include <stdio.h>

typedef long long int64;

// прямоугольник
struct rectangle {
  int64 x, y; // ширина, высота
};

struct rectangle r;

double area(struct rectangle r) {
  return (double)r.x * (double)r.y;
}

int main() {
  r.x = 5;
  r.y = 10;

  double p;
  p = area(r);

  printf("%f\n", p);

  return 0;
}

