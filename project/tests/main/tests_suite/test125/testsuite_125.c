#include <stdio.h>
int
test()
{
	return 3-3;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}