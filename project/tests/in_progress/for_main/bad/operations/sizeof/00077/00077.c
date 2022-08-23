#include <stdio.h>

int
foo(int x[100])
{
	int y[100];
	int *p;
	
	y[0] = 2000;
	
	if(x[0] != 1000)
	{
		printf("x[0] != 1000\n");
		return 1;
	}
	printf("x[0] == 1000\n");

	p = x;
	
	if(p[0] != 1000)
	{
		return 2;
	}
	printf("p[0] == 1000\n");

	p = y;
	
	if(p[0] != 2000)
	{
		return 3;
	}
	printf("p[0] == 2000\n");
/*
	if(sizeof(x) != sizeof(void*))
	{
		printf("sizeof(x) != sizeof(void*)\n");
		return 4;
	}
	printf("sizeof(x) == sizeof(void*)\n");

	if(sizeof(y) <= sizeof(x))
	{
		printf("sizeof(y) <= sizeof(x)\n");
		return 5;
	}
	printf("sizeof(y) == sizeof(x)\n");
*/
	return 0;
}

int
test()
{
	int x[100];
	x[0] = 1000;
	
	return foo(x);
}

int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
