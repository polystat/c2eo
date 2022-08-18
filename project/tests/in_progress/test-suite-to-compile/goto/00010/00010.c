int
test()
{
	start:
		goto next;
		return 1;
	success:
		return 0;
	next:
	foo:
		goto success;
		return 1;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
