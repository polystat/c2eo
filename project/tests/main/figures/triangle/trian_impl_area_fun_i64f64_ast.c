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

// треугольник
struct triangle {
  int64 a, b, c; // ширина, высота
};

struct triangle t;

double area(struct triangle *t) {
  double p;
  p = ((*t).a + (*t).b + (*t).c) / 2.0;
  return sqrt_newton(p * (p - (*t).a) * (p - (*t).b) * (p - (*t).c));
}

int main() {
  t.a = 30;
  t.b = 40;
  t.c = 50;

  double a;
  a = area(&t);

  printf("%f\n", a);
  a;

  return 0;
}

