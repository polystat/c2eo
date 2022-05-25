#include <stdio.h>
int
test()
{
	int x, *p, **pp;
	
	x = 0;
	p = &x;
	pp = &p;
	return **pp;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}