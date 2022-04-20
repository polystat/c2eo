#include <stdio.h>
int
test()
{
	int x = 0;
	int y = 1;
	if(x ? 1 : 0)
		return 1;
	if(y ? 0 : 1)
		return 2;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}