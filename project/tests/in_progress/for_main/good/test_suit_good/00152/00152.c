#undef  line
#define line 1000

#line line
#if 1000 != __LINE__
	#error "  # line line" not work as expected
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
