#include <stdio.h>
#define x(y) #y

int
test(void)
{
	char *p;
	p = x(hello)  " is better than bye";

	return (*p == 'h') ? 0 : 1;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}