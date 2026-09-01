#include <stdio.h>

typedef long long int64;

double abs_double(double x) {
  if(x < 0.0) {
    x = -x;
  }
  return x;
}

double sqrt_newton(double x) {
  double dividend;
  dividend = x;
  double last;
  if (x > 0.00000001) {
    do {
      last = x;
      x = (x + dividend / x) * 0.5;
    } while(abs_double(x - last) > 1e-9); // precision
  }
  return x;
}

double const pi = 3.141592;

// круг
struct circle {
  int64 r;  // радиус
};

double circle_area(struct circle c) {
  return pi * c.r * c.r;
}

// прямоугольник
struct rectangle {
  int64 x, y; // ширина, высота
};

double rect_area(struct rectangle r) {
  return (r.x * r.y);
}

// треугольник
struct triangle {
  int64 a, b, c; // ширина, высота
};

double trian_area(struct triangle t) {
  double p;
  p = (t.a + t.b + t.c) / 2.0;
  return sqrt_newton(p * (p - t.a) * (p - t.b) * (p - t.c));
}

struct figure {
  int64 key;
  union {
    struct circle c;
    struct rectangle r;
    struct triangle t;
  };
};

double figure_area(struct figure f) {
  double p;
  if(f.key == 1) {
    p = circle_area(f.c);
  }
  else if(f.key == 2) {
    p = rect_area(f.r);
  }
  else if(f.key == 3) {
    p = trian_area(f.t);
  }
  else {
    p = 0.0;
  }
  return p;
}

struct figure f;

int main() {
  f.c.r = 6;
  f.r.y = 8;
  f.t.c = 10;

  double p;
  p = circle_area(f.c);
  printf("%f\n", p);

  p = rect_area(f.r);
  printf("%f\n", p);

  p = trian_area(f.t);
  printf("%f\n", p);

  f.key = 0;
  p = figure_area(f);
  printf("%f\n", p);

  f.key = 1;
  p = figure_area(f);
  printf("%f\n", p);

  f.key = 2;
  p = figure_area(f);
  printf("%f\n", p);

  f.key = 3;
  p = figure_area(f);
  printf("%f\n", p);

  f.key = 10;
  p = figure_area(f);
  printf("%f\n", p);

  return 0;
}

