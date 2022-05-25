#include <stdio.h>
int
test()
{
	int x;
	
	x = 4;
	if(!x != 0)
		return 1;
	if(!!x != 1)
		return 1;
	if(-x != 0 - 4)
		return 1;
	return 0;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}