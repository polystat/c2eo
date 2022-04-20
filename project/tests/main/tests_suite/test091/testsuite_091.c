#include <stdio.h>
#ifndef DEF
int x = 0;
#endif

#define DEF

#ifndef DEF
X
#endif

int
test()
{
	return x;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}