#include <stdio.h>
struct st {
  int a;
  struct st2 {
    int x, y;
  } b;
} a/* = {1, {2, 3}}*/;
int b = 17;

int main() {
  a.a = 1;
  a.b.x = 2;
  a.b.y = 3;

  a.b.x;
  printf("%d\n", a.b.x);
  b;
  printf("%d\n", b);
  return 0;
}