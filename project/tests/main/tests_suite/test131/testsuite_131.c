#include <stdio.h>
struct S1 { int x; };
struct S2 { struct S1 s1; };

int
test()
{
	struct S2 s2;
	s2.s1.x = 1;
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}