#include <stdio.h>
int
test()
{
	int x;
	
	x = 1;
	x = x | 4;
	return x - 5;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}