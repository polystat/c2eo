#include <stdio.h>
//extern int x;
int x;
extern int x;

int
test()
{
	return x;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}
