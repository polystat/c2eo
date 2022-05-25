#include <stdio.h>
#define F(a, b) a
int
test()
{
	return F(, 1) 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}