#include <stdio.h>
int
foo(int a, int b)
{
	return 2 + a - b;
}

int
test()
{
	return foo(1, 3);
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}