#include <stdio.h>
int
test()
{

	struct S { int x; int y; } s;
	struct S *p;

	p = &s;	
	s.x = 1;
	p->y = 2;
	return p->y + p->x - 3; 
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}