int
test()
{
	int x;
	int *p;
	
	x = 1;
	p = &x;
	p[0] = 0;
	return x;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
