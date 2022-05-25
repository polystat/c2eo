#include <stdio.h>
#define FOO 0

int test()
{
	return FOO;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}