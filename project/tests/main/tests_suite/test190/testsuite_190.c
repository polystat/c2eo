#include <stdio.h>
int
f()
{
	return 100;
}

int
test()
{
	if (f() > 1000)
		return 1;
	if (f() >= 1000)
		return 1;
	if (1000 < f())
		return 1;
	if (1000 <= f())
		return 1;
	if (1000 == f())
		return 1;
	if (100 != f())
		return 1;
	return 0;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}