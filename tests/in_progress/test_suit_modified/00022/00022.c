typedef int x;

int
test()
{
	x v;
	v = 0;
	return v;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
