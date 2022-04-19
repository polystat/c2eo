#include <stdio.h>
int
test()
{
	int a = 0;
	float f = a + 1;

	return f == a;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}