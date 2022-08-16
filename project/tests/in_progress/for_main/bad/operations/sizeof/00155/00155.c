
int
test(void)
{
	sizeof((int) 1);
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
