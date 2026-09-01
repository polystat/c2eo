int x;
int x = 3;
int x;

int main();

void *
foo()
{
	return &main;
}

int
test()
{
	if (x != 3)
		return 0;

	x = 0;
	return x;
}


#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
