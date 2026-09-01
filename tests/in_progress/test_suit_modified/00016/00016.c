int
test()
{
	int arr[2];
	int *p;
	
	p = &arr[1];
	*p = 0;
	return arr[1];
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
