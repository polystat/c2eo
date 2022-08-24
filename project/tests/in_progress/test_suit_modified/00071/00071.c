#define X 1
#undef X

#ifdef X
FAIL
#endif

int
test()
{
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
