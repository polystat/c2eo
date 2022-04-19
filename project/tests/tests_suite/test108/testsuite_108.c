#include <stdio.h>
#define X 6 / 2

int
test()
{
	return X - 3;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}