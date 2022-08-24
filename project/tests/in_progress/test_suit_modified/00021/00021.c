int
foo(int a, int b)
{
	return 2 + a - b;
}

int
test()
{
	return foo(1, 3);
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
