#include <stdio.h>
#if defined(FOO)
int a;
#elif !defined(FOO) && defined(BAR)
int b;
#elif !defined(FOO) && !defined(BAR)
int c;
#else
int d;
#endif

int
test(void)
{
	return c;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}