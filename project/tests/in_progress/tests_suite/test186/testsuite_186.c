#include <stdio.h>

int
test(void)
{
	sizeof((int) 1);
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}