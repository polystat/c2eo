int a[] = {1, 2, 3, 4};

int
test()
{
	if (sizeof(a) != 4*sizeof(int))
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
