#define ADD(X, Y) (X + Y)


int
test()
{
	return ADD(1, 2) - 3;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
