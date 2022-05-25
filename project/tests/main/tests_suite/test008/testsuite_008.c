#include <stdio.h>
int
test()
{
	int x;
	
	x = 4;
	return x - 4;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}