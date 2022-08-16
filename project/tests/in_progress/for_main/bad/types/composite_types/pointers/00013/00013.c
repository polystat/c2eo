int
test()
{
	int x;
	int *p;
	
	x = 0;
	p = &x;
	return p[0];
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
