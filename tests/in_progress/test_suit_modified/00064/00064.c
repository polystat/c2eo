#define X 6 / 2

int
test()
{
	return X - 3;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
