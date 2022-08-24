int test()
{
	int x[] = { 1, 0 };
	return x[1];
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
