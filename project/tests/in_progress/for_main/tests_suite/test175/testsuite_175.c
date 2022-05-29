#include <stdio.h>
int
test()
{
	return "abc" == (void *)0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}