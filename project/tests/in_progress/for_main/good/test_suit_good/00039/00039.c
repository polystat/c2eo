int
test()
{
	void *p;
	int x;
	
	x = 2;
	p = &x;
	
	if(*((int*)p) != 2)
		return 1;
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
