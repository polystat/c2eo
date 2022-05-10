#include <stdio.h>

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

int main() {
  double x;
  x = 741;
  printf("%lf\n", x);
  x;

  double result;
  result = sqrt_newton(741);
  printf("%lf\n", result);
  result;

  result = sqrt_newton(2.0);
  printf("%lf\n", result);
  result;

  return 0;
}
