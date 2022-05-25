#include <stdio.h>
int
test()
{
	int x;
	int *p;
	
	x = 4;
	p = &x;
	*p = 0;

	return *p;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}