#include <stdio.h>
int (*fptr)() = 0;


int
test()
{
	if (fptr)
		return 1;
	return 0;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}