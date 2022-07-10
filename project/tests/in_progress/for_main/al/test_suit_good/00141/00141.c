#define CAT(x,y) x ## y
#define XCAT(x,y) CAT(x,y)
#define FOO foo
#define BAR bar

int
test(void)
{
	int foo, bar, foobar;

	CAT(foo,bar) = foo + bar;
	XCAT(FOO,BAR) = foo + bar;
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
