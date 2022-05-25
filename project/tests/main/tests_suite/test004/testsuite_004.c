#include <stdio.h>
int
f(int f)
{
	return f;
}

int
test()
{
	return f(0);
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}