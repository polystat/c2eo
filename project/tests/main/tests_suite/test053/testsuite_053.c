#include <stdio.h>
#define ADD(X, Y) (X + Y)


int
test()
{
	return ADD(1, 2) - 3;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}