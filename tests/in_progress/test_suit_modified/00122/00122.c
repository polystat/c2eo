#define F(a, b) a
int
test()
{
	return F(, 1) 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
