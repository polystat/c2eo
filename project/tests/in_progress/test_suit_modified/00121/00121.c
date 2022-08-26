int f(int a), g(int a), a;

int
test()
{
	return f(1) - g(1);
}

int
f(int a)
{
	return a;
}

int
g(int a)
{
	return a;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
