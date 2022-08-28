double x = 100.0;

int
test()
{
	return x < 1;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
