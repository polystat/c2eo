int
test()
{
	int x;
	
	x = 1;
	x = x & 3;
	return x - 1;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
