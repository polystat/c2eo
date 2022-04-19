#include <stdio.h>
extern int x;

int test()
{
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}