#include <stdio.h>

enum key {RECTANGLE, TRIANGLE};

struct base {
  enum key k;
};

struct rectangle {
  int x, y;
};

struct triangle {
  int a, b, c;
};

struct figure {
  enum key k;
  union {
    struct {int x, y;};
    struct {int a, b, c;};
  };
};

int main() {
  struct figure f;
  f.x = 5;
  f.y = 10;
  f.c = 8;

  printf("%d\n", f.a);
  printf("%d\n", f.b);
  printf("%d\n", f.c);

  return 0;
}
