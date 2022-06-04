#include <stdio.h>
int test()
{
	int x[] = { 1, 0 };
	return x[1];
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}