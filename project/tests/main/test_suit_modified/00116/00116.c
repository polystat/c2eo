int
f(int f)
{
	return f;
}

int
test()
{
	return f(0);
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
