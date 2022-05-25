#include <stdio.h>
int
test()
{
	int x;
	int y;
	x = y = 0;
	return x;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}