#include <stdio.h>
int
test()
{
	if(0 ? 1 : 0)
		return 1;
	if(1 ? 0 : 1)
		return 2;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}