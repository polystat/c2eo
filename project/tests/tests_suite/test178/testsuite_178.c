#include <stdio.h>
extern int x;
int x;

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