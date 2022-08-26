int
test()
{
	int x;

	x = 50;
	do 
		x = x - 1;
	while(x);
	return x;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
