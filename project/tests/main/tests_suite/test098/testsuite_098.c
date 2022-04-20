#include <stdio.h>
typedef int x;

int
test()
{
	x v;
	v = 0;
	return v;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}