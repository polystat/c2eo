#include <stdio.h>

// прямоугольник
struct rectangle {
  int x, y; // ширина, высота
};

// треугольник
struct triangle {
  int a, b, c; // стороны
};

// фигура
struct figure {
  int key;
  union {
    struct rectangle r;
    struct triangle t;
  };
};

int main() {
//   f.r = 6;
//   f.y = 8;
//   f.c = 10;
//
//   double p;
//   p = circle_perimeter(f.r);
//   printf("%f\n", p);
//
//   p = rect_perimeter(f.x, f.y);
//   printf("%f\n", p);
//
//   p = trian_perimeter(f.a, f.b, f.c);
//   printf("%f\n", p);
//
//   f.key = 0;
//   p = figure_perimeter(&f);
//   printf("%f\n", p);
//
//   f.key = 1;
//   p = figure_perimeter(&f);
//   printf("%f\n", p);
//
//   f.key = 2;
//   p = figure_perimeter(&f);
//   printf("%f\n", p);
//
//   f.key = 3;
//   p = figure_perimeter(&f);
//   printf("%f\n", p);
//
//   f.key = 10;
//   p = figure_perimeter(&f);
//   printf("%f\n", p);

  return 0;
}
