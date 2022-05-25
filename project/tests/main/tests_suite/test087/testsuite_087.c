#include <stdio.h>
#define X 1
#undef X

#ifdef X
FAIL
#endif

int
test()
{
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}