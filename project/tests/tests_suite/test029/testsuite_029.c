#include <stdio.h>
int
test()
{
	struct { int x; int y; } s;
	
	s.x = 3;
	s.y = 5;
	return s.y - s.x - 2; 
}
int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}