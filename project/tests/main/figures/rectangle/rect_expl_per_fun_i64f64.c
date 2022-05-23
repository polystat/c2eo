#include <stdio.h>

typedef long long int64;

// прямоугольник
struct rectangle {
  int64 x, y; // ширина, высота
};

struct rectangle r;

double perimeter(struct rectangle r) {
  return (double)(2.0 * (r.x + r.y));
}

int main() {
  r.x = 5;
  r.y = 10;

  double p;
  p = perimeter(r);

  printf("%f\n", p);
  p;

  return 0;
}

