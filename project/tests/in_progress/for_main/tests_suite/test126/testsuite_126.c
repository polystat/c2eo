#include <stdio.h>
int
test()
{
	start:
		goto next;
		return 1;
	success:
		return 0;
	next:
	foo:
		goto success;
		return 1;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}