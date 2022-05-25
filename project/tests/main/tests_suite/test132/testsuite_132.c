#include <stdio.h>
int f(int a), g(int a), a;


int
test()
{
	return f(1) - g(1);
}

int
f(int a)
{
	return a;
}

int
g(int a)
{
	return a;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}