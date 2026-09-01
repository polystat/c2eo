int
test()
{
	int x;

	x = 50;
	while (x)
		x = x - 1;
	return x;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
