typedef int myint;
myint x = (myint)1;

int
test(void)
{
	return x-1;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
