#include <stdio.h>
int
test()
{
	struct { int x; } s = { 0 };
	return s.x;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}