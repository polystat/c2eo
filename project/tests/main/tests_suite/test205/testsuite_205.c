#include <stdio.h>
#if 1
int x = 0;
#endif

#if 0
int x = 1;
#if 1
 X
#endif
#ifndef AAA
 X
#endif
#endif

int test()
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