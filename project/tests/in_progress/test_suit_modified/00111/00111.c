int
test()
{
	short s = 1;
	long l = 1;

	s -= l;
	return s;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
