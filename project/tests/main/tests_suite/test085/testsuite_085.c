#include <stdio.h>
#define NULL ((void*)0)
#define NULL ((void*)0)

#define FOO(X, Y) (X + Y + Z)
#define FOO(X, Y) (X + Y + Z)

#define BAR(X, Y, ...) (X + Y + Z)
#define BAR(X, Y, ...) (X + Y + Z)

int
test()
{
	return 0;
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  return 0;
}