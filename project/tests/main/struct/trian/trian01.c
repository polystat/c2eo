#include <stdio.h>

struct Triangle{int a, b, c;} t = {.a = 3, .c = 5};


int main() {
  t.b = 4;
  printf("t.a = %d, t.b = %d, t.c = %d\n", t.a, t.b, t.c);
  return 0;
}
