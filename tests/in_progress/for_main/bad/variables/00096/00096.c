int x, x = 3, x;

int
test()
{
	if (x != 3)
		return 3;

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
