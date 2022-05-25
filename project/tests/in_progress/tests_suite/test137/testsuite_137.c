#include <stdio.h>
int
test()
{
	char *p;
	
	p = "hello";
	return p[0] - 104;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}