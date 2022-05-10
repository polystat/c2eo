#include <stdio.h>

typedef long long int64;

double const pi = 3.141592;

// круг
struct circle {
  int64 r;  // радиус
};

double circle_perimeter(struct circle c) {
  return 2.0 * pi * c.r;
}

// прямоугольник
struct rectangle {
  int64 x, y; // ширина, высота
};

double rect_perimeter(struct rectangle r) {
  return (2.0 * (r.x + r.y));
}

// треугольник
struct triangle {
  int64 a, b, c; // ширина, высота
};

double trian_perimeter(struct triangle t) {
  return t.a + t.b + t.c;
}

struct figure {
  int key;
  union {
    struct circle c;
    struct rectangle r;
    struct triangle t;
  };
};

struct figure f;


int main() {
  f.c.r = 6;
  f.r.y = 8;
  f.t.c = 10;

  double p;
  p = circle_perimeter(f.c);
  printf("%f\n", p);
  p;

  p = rect_perimeter(f.r);
  printf("%f\n", p);
  p;

  p = trian_perimeter(f.t);
  printf("%f\n", p);
  p;

  return 0;
}

