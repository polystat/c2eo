#include <stdio.h>

struct q {
  int b;
  struct q2 {
    struct q3 {
      int h;
    } h;
  } a;
  struct q4 {
    int h;
  } h;
  unsigned long long q;
  struct q4 y;
};

int main() {
  struct q a /*= {1, 2, 3}*/;
  a.b = 1;
  a.a.h.h = 2;
  a.h.h = 3;


  a.y.h = 7;
  printf("%d\n", a.y.h);
  return 0;
}