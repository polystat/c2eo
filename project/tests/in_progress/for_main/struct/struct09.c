
#include <stdio.h>
struct st {
  int a;
  struct st2 {
    int x, y;
  } b;
} a/* = {1, {2, 3}}*/;



struct st2 b/* = {1,2}*/;

int main() {
  b.x = 1;
  b.y = 2;
  a.a=1;
  a.b.x=2;
  a.b.y=3;


  a.a;
  printf("%d\n", a.a);
  a.b.x;
  printf("%d\n", a.b.x);
  a.b.y;
  printf("%d\n", a.b.y);

  a.a = a.a - 17;
  a.b.x = a.b.x + 12;
  a.b.y = a.a * a.b.x;

  a.a;
  printf("%d\n", a.a);
  a.b.x;
  printf("%d\n", a.b.x);
  a.b.y;
  printf("%d\n", a.b.y);

  return 0;
}