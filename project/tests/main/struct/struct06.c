#include <stdio.h>
struct q {
  int a, b, c, d;
} a/* = {1, 2, 3, 4}*/,
    b/* = {2, 3, 4, 5}*/;

int main() {
  a.a=1;
  a.b=2;
  a.c=3;
  a.d=4;

  b.a=a.a+1;
  b.b=a.b+1;
  b.c=a.c+1;
  b.d=a.d+1;


  a.b;
  printf("%d\n", a.b);
  b.b;
  printf("%d\n", b.b);
  a = b;
  a.b;
  printf("%d\n", a.b);
  return 0;
}