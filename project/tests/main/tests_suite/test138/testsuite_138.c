#include <stdio.h>
int
test()
{
	short s = 1;
	long l = 1;

	s -= l;
	return s;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}