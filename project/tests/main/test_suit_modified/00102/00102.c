int
test()
{
	int x;
	
	x = 1;
	if ((x << 1) != 2)
		return 1;
	
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
