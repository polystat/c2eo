#include <stdio.h>
int
test()
{
	void *p;
	int x;
	
	x = 2;
	p = &x;
	
	if(*((int*)p) != 2)
		return 1;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}