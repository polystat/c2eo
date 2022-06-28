int x;

int
test()
{
	x = 0;
	return x;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
