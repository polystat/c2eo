#include <stdio.h>
int
test()
{
	struct S { struct S *p; int x; } s;
	
	s.x = 0;
	s.p = &s;
	return s.p->p->p->p->p->x;
}

int main () {
  int x;
  x = test();
  printf("%d\n", x);
  x;
  return 0;
}