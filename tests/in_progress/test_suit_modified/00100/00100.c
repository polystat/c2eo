int
foo(void)
{
	return 0;
}

int
test()
{
	return foo();
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
