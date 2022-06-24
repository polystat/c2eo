#include <stdio.h>
int x;
int x = 3;
int x;

int test();

int
test()
{
	if (x != 3)
		return 0;

	x = 0;
	return x;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}

void *
foo()
{
	return &main;
}

