#include <stdio.h>
int
test()
{
	struct T { int x; };
	{
		struct T s;
		s.x = 0;
		return s.x;
	}
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}