#include <stdio.h>

typedef long long int64;

struct rectangle {
  int64 x, y;
};

int64 a;

int64 x;
int64 y = 7;

extern int64 a;
extern int64 a;

int main() {
  x = a;
  a = y;

  printf("%lld\n", a);
  a;

  return 0;
}

extern int64 a;
extern int64 a;
