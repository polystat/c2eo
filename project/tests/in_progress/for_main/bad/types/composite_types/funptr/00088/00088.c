int (*fptr)() = 0;


int
test()
{
	if (fptr)
		return 1;
	return 0;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
