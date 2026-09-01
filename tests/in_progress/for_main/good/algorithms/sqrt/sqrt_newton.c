#include <stdio.h>

double my_abs(double x) {
  if(x < 0.0) {
    return -x;
  }
  return x;
}

double sqrt_newton(double x) {
    if (x == 0.0) {
        printf("%f\n", x);
        return x;
    }
    double dividend = x;
    double val = x;
    double last;
    int i = 0;
    do {
        ++i;
        last = val;
        val = (val + dividend / val) * 0.5;
        printf("%d: %f\n", i, val);
    } while(my_abs(val - last) > 1e-6); // precision
    return val;
}

int main() {
    double x = 2.0;
    printf("start value = %f\n", x);
    x = sqrt_newton(x);
    printf("result = %f\n", x);
}
