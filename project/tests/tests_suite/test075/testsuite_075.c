#include <stdio.h>
int
test()
{
	if ('a' != 97)
		return 1;
		
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}