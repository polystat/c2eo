#include <stdio.h>
typedef int myint;
myint x = (myint)1;

int
test(void)
{
	return x-1;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}