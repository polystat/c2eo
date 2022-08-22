int
test()
{
	struct S { struct S *p; int x; } s;
	
	s.x = 0;
	s.p = &s;
	return s.p->p->p->p->p->x;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
