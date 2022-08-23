int
test()
{
	int x;
	
	x = 1;
	x = x * 10;
	x = x / 2;
	x = x % 3;
	return x - 2;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
