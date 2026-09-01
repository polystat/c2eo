int
test()
{
	if(0 ? 1 : 0)
		return 1;
	if(1 ? 0 : 1)
		return 2;
	return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
