#include <stdio.h>

typedef long long int64;

double const pi = 3.141592;

double circle_perimeter(int64 r) {
  return 2.0 * pi * r;
}

double rect_perimeter(int64 x, int64 y) {
  return (2.0 * (x + y));
}

double trian_perimeter(int64 a, int64 b, int64 c) {
  return a + b + c;
}

struct figure {
  int64 key;
  union {
    // круг
    struct {
      int64 r;  // радиус
    };
    // прямоугольник
    struct {
      int64 x, y; // ширина, высота
    };
    // треугольник
    struct {
      int64 a, b, c; // ширина, высота
    };
  };
};

double figure_perimeter(struct figure *f) {
  double p;
  if((*f).key == 1) {
    p = circle_perimeter((*f).r);
  }
  else if((*f).key == 2) {
    p = rect_perimeter((*f).x, (*f).y);
  }
  else if((*f).key == 3) {
    p = trian_perimeter((*f).a, (*f).b, (*f).c);
  }
  else {
    p = 0.0;
  }
  return p;
}

struct figure f;

int main() {
  f.r = 6;
  f.y = 8;
  f.c = 10;

  double p;
  p = circle_perimeter(f.r);
  printf("%f\n", p);

  p = rect_perimeter(f.x, f.y);
  printf("%f\n", p);

  p = trian_perimeter(f.a, f.b, f.c);
  printf("%f\n", p);

  f.key = 0;
  p = figure_perimeter(&f);
  printf("%f\n", p);

  f.key = 1;
  p = figure_perimeter(&f);
  printf("%f\n", p);

  f.key = 2;
  p = figure_perimeter(&f);
  printf("%f\n", p);

  f.key = 3;
  p = figure_perimeter(&f);
  printf("%f\n", p);

  f.key = 10;
  p = figure_perimeter(&f);
  printf("%f\n", p);

  return 0;
}
