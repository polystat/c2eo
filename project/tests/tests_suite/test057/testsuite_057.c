#include <stdio.h>
double x = 100.0;

int
test()
{
	return x < 1;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}