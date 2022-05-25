#include <stdio.h>
int
test()
{
	int x;
	int *p;
	
	x = 0;
	p = &x;
	return p[0];
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}