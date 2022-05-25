#include <stdio.h>
int
test()
{
	short x;
	
	x = 0;
	x = x + 1;
	if (x != 1)
		return 1;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}