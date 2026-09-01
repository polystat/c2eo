struct S
{
	int	(*fptr)();
};

int
foo()
{
	return 0;
}

int
test()
{
	struct S v;
	
	v.fptr = foo;
	return v.fptr();
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
