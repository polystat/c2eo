#include <stdio.h>
int foo(void);
int foo(void);
#define FOO 0

int
test()
{
	return FOO;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}