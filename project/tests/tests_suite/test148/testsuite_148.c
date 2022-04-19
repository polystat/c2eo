#include <stdio.h>
int
foo(void)
{
	return 0;
}

int
test()
{
	return foo();
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}