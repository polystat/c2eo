#include <stdio.h>
union { int a; int b; } u;
int main () {

  u.a = 1;
  u.b = 3;
  printf("%d\n", u.a);
  printf("%d\n", u.b);
  return 0;
}