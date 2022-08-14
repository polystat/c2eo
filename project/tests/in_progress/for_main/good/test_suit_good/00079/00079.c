#define x(y) ((y) + 1)

int
test()
{
	int x;
	int y;
	
	y = 0;
	x = x(y);
	
	if(x != 1)
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
