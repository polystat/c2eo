#define x(y) #y

int
test(void)
{
	char *p;
	p = x(hello)  " is better than bye";

	return (*p == 'h') ? 0 : 1;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
