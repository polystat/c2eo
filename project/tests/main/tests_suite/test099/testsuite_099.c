#include <stdio.h>
int
test()
{
	return (2 + 2) * 2 - 8;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}