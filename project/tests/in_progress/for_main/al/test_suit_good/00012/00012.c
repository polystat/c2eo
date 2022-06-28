int
test()
{
	return (2 + 2) * 2 - 8;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
