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
  circle.pi = pi;
  circle.r = r;
  circle.two = two;

  double tmp = circle.pi * circle.r;
  circle.p = circle.two * tmp;
  printf("%f\n", circle.p);
  circle.s = tmp * circle.r;
  printf("%f\n", circle.s);

  return 0;
}
