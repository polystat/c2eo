#ifndef DEF
int x = 0;
#endif

#define DEF

#ifndef DEF
X
#endif

int
test()
{
	return x;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
