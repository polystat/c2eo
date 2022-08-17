int
test()
{
	int a = 0;
	float f = a + 1;

	return f == a;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
