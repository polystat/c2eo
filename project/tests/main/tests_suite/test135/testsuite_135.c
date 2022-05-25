#include <stdio.h>
typedef struct { int x; int y; } s;

s v;

int
test()
{
	v.x = 1;
	v.y = 2;
	return 3 - v.x - v.y;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}