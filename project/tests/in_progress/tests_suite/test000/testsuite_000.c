#include <stdio.h>
int x = 5;
long y = 6;
int *p = &x;

int
test()
{
	if (x != 5) 
		return 1;
	if (y != 6)
		return 2;
	if (*p != 5)
		return 3;
	return 0;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}