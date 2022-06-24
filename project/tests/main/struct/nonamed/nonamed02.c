#include <stdio.h>

struct figure {
  long k;
  union {
    struct {long x, y;};
    struct {long a, b, c;};
  };
};

double rect_perimeter(long x, long y) {
  return (2.0 * (x + y));
}


int main() {
  struct figure f;
  f.x = 5;
  f.y = 10;
  f.c = 8;

  printf("%ld\n", f.a);
  printf("%ld\n", f.b);
  printf("%ld\n", f.c);

  double p;
  p = rect_perimeter(f.a, f.b);
  printf("%f\n", p);

  return 0;
}
