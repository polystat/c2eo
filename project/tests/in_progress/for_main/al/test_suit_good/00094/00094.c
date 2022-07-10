extern int x;

int test()
{
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
