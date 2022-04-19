#include <stdio.h>
int
f2(int c, int b)
{
	return c - b;
}

int (*
f1(int a, int b))(int c, int b)
{
	if (a != b)
		return f2;
	return 0;
}

int
test()
{
	int (* (*p)(int a, int b))(int c, int d) = f1;


	return (*(*p)(0, 2))(2, 2);
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}