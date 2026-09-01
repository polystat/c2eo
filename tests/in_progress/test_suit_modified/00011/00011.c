int
test()
{
	int x;
	int y;
	x = y = 0;
	return x;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
