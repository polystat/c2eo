#include <stdio.h>
int
test()
{
	int x;
	
	x = 1;
	x = x & 3;
	return x - 1;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}