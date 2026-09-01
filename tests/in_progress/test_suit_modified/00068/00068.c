#if 0
X
#elif 1
int x = 0;
#else
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
