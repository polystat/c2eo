#include <stdio.h>
int x, x = 3, x;

int
test()
{
	if (x != 3)
		return 0;

	x = 0;
	return x;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}