#if 1
int x = 0;
#endif

#if 0
int x = 1;
#if 1
 X
#endif
#ifndef AAA
 X
#endif
#endif

int test()
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
