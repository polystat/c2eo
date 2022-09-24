#include <stdio.h>

int
foo(int x[100])
// foo(int x[])
// foo(int *x)
{
	int y[100];
	int *p;

	y[0] = 2000;
	printf("y[0] = %d\n", y[0]);
	printf("x[0] = %d\n", *x);
	// 	printf("x[0] = %d\n", x[0]);

// 	if(x[0] != 1000)
	if(*x != 1000)
	{
		printf("x[0] != 1000\n");
		return 1;
	}
	printf("x[0] == 1000\n");

	p = x;

// 	if(p[0] != 1000)
	if(*p != 1000)
		{
		return 2;
	}
	printf("p[0] == 1000\n");

// 	p = y;
	p = &y[0];

// 	if(p[0] != 2000)
	if(*p != 2000)
	{
		return 3;
	}
	printf("p[0] == 2000\n");

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
	printf("sizeof(y) = %d, sizeof(x) = %d\n", sizeof(y), sizeof(x));

	return 0;
}

int
test()
{
// 	int x = 5;
 	int x[100];
	x[0] = 1000;
	
	return foo(x);
// 	return foo(x);
}

int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
