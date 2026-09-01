#include <stdio.h>

struct circle{
  double pi, r, p, s, two;
} circle;

double pi = 3.14192;
static double r = 10.0;
double p = 0.0;
double s = 0.0;
double two = 2.0;

int main() {
  printf("%f\n", pi);
  printf("%f\n", r);
  printf("%f\n", p);
  printf("%f\n", s);
  printf("%f\n", two);
  circle.pi = pi;
  printf("%f\n", circle.pi);
  circle.r = r;
  printf("%f\n", circle.r);
  circle.two = two;
  printf("%f\n", circle.two);

  double tmp;
  tmp = circle.pi * circle.r;
  printf("%f\n", tmp);
  circle.p = circle.two * tmp;
  printf("%f\n", circle.p);
  circle.s = tmp * circle.r;
  printf("%f\n", circle.s);

  return 0;
}
