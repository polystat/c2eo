int
test()
{
	int x;
	
	x = 1;
	x = x | 4;
	return x - 5;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
