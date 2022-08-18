int foo(void);
int foo(void);
#define FOO 0

int
test()
{
	return FOO;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
