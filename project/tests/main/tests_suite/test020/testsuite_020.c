#include <stdio.h>
int x;

int
test()
{
	x = 0;
	return x;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}