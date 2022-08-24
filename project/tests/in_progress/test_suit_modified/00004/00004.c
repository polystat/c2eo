int
test()
{
	int x;
	int *p;
	
	x = 4;
	p = &x;
	*p = 0;

	return *p;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
