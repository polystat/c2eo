#include <stdio.h>
int
test()
{
	int x;

	x = 50;
	while (x)
		x = x - 1;
	return x;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}