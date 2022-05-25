#include <stdio.h>
#if 0
X
#elif 1
int x = 0;
#else
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
  return 0;
}