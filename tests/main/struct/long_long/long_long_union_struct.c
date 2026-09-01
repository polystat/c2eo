#include <stdio.h>

struct figure {
  long long k;
  union {
    struct {long long x, y;};
    struct {long long a, b, c;};
  };
};

double rect_perimeter(long long x, long long y) {
  return (2.0 * (x + y));
}


int main() {
  struct figure f;
  f.x = 5;
  f.y = 10;
  f.c = 8;

  printf("%lld\n", f.a);
  printf("%lld\n", f.b);
  printf("%lld\n", f.c);

  double p;
  p = rect_perimeter(f.a, f.b);
  printf("%f\n", p);

  return 0;
}
