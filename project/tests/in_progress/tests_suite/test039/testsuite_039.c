#include <stdio.h>
int
test()
{
	int x;
	int *p;
	
	x = 1;
	p = &x;
	p[0] = 0;
	return x;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}