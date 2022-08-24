int
test()
{
	int x;
	
	x = 4;
	return x - 4;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
