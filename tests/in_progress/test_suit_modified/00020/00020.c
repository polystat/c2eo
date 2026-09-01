int
test()
{
	int x, *p, **pp;
	
	x = 0;
	p = &x;
	pp = &p;
	return **pp;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
